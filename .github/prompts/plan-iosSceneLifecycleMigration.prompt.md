# Plan: Add UIScene lifecycle support to Core ANE delegate proxy

Extend the existing swizzling architecture in `DTCAppDelegateProxy` to also swizzle `UIWindowSceneDelegate`/`UISceneDelegate` methods, since UIKit routes certain events to the scene delegate instead of the app delegate once an app adopts `UIApplicationSceneManifest`. Reuse the same NSNotificationCenter notification names already declared in `CoreNativeExtension`/`DTNotifications`, so **no changes are needed in corelibraries or in downstream extensions** that consume `DTNotifications` — they keep working unmodified whether the host app is AppDelegate-only or Scene-based.

**Why this works:** UIKit calls scene-based delegate methods *instead of* their `UIApplicationDelegate` counterparts only for a subset of events (continueUserActivity, openURL, shortcut items); `didFinishLaunchingWithOptions` and all push/local notification callbacks stay on the app delegate regardless. So the fix is additive: keep existing AppDelegate swizzling untouched, and add scene delegate swizzling that posts the *same* notifications with equivalent payload shape.

## Critical issues / edge cases identified (must address in implementation)

1. **Cold-start timing race — `connectionOptions` is missed.** On a cold launch via Universal Link, custom URL, or shortcut item, UIKit delivers the incoming URL/activity/shortcut inside `scene:willConnectToSession:options:`'s `connectionOptions`, and does **not** separately call `scene:openURLContexts:` / `scene:continueUserActivity:` / the shortcut method for that initial launch. Must swizzle `scene:willConnectToSession:options:` and extract `connectionOptions.URLContexts`, `connectionOptions.userActivities`, and `connectionOptions.shortcutItem`, posting the same `DTCore*` notifications from there.
2. **`scene.delegate` may be `nil` when `UISceneWillConnectNotification` fires.** UIKit may not have instantiated/bound the delegate yet. Fallback: resolve `scene.session.configuration.delegateClass` and swizzle that class directly; if unavailable, KVO-observe `scene.delegate` until assigned.
3. **Optional protocol methods need `class_addMethod`, not a raw exchange.** If the host's scene delegate class doesn't implement an optional method (e.g. `scene:openURLContexts:`), a naive `class_getInstanceMethod` + `method_setImplementation` walks the superclass chain and can swizzle a shared superclass (or `NSObject`) instead of the target class, corrupting unrelated classes. The swizzle helper must add the method to the class itself when absent, falling back to exchange only when it's genuinely defined directly on that class.
4. **Multiple simultaneous `openURLContexts` (multi-window iPadOS/visionOS).** Posting one `DTCoreOpenURLOptionsNotification` per context is correct, but downstream consumers assuming a single app-wide open event may need to dedupe by URL/timestamp — call this out to consumers rather than trying to coalesce here.
5. **`originalMethods_` thread safety.** Scene connection callbacks can fire concurrently across multiple scenes/threads; the static dictionary must be synchronized (`@synchronized` or a serial dispatch queue) for both reads and writes.

## Steps

1. ✅ **DONE — Harden the generic swizzle helper before adding scene support** (*prerequisite*):
   - Fixed `+swizzle:implementation:class:` in `DTCAppDelegateProxy.m` to try `class_addMethod` first (using the existing method's type encoding), only falling back to `method_setImplementation`/exchange when `class_addMethod` fails (meaning the selector is defined directly on `class` itself, not inherited) — addresses Critical Issue #3.
   - Wrapped all reads/writes to the static `originalMethods_` dictionary (`setupAppDelegateProxy:`'s init/dedup checks, `originalImplementation:class:`, `storeOriginalImplementation:selector:class:`) in `@synchronized (self)` — addresses Critical Issue #5.

2. **Add scene delegate swizzling to `DTCAppDelegateProxy`** (reuse the hardened helpers from step 1 — no need for a separate class/file since they're already class-generic).
   - New private method `+setupSceneDelegateProxyForScene:(UIScene *)scene`, guarded by `API_AVAILABLE(ios(13.0))` / `if (@available(iOS 13.0, *))`, that:
     - Gets `scene.delegate`. **If `nil`** (Critical Issue #2), fall back to resolving `scene.session.configuration.delegateClass` and swizzle that class directly so the swizzle is ready before UIKit assigns the delegate instance; additionally KVO-observe `scene.delegate` as a safety net.
     - Checks the delegate's class hasn't already been swizzled (reuse the now-synchronized `originalMethods_` dict keyed by class name).
     - Swizzles `scene:willConnectToSession:options:` (Critical Issue #1) — extracts `connectionOptions.URLContexts`, `connectionOptions.userActivities`, and `connectionOptions.shortcutItem`, posting the equivalent `DTCoreOpenURLOptionsNotification` / `DTCoreApplicationContinueUserActivityNotification` / `DTCorePerformActionForShortcutItemNotification` for each, covering the cold-launch case.
     - Swizzles `scene:continueUserActivity:` → posts `DTCoreApplicationContinueUserActivityNotification` with `userActivity` key (same shape as today; no restorationHandler needed — the scene API has none, and the current AppDelegate handler doesn't forward it into userInfo either).
     - Swizzles `scene:openURLContexts:` (takes `NSSet<UIOpenURLContext *> *`) → for each context, build an options dictionary from `UIOpenURLContext.options` (a `UISceneOpenURLOptions`) mapping `sourceApplication`, `annotation`, `openInPlace` to match the existing `NSDictionary` shape used by `application:openURL:options:`, then post `DTCoreOpenURLOptionsNotification` per context with `url`/`options` keys (same as current `DTCoreOpenURLOptions` poster). One notification per context is intentional (Critical Issue #4) — document that multi-context delivery is possible.
     - Swizzles `windowScene:performActionForShortcutItem:completionHandler:` (only if delegate conforms to `UIWindowSceneDelegate`) → posts `DTCorePerformActionForShortcutItemNotification` with `shortcutItem`/`completionHandler` keys (same shape as today).
   - Shortcut-item-related swizzles guarded by `#if !TARGET_OS_TV` to match existing exclusions, consistent with current AppDelegate swizzling.

3. **Hook scene discovery into `setupAppDelegateProxy:`** (*depends on step 2*):
   - At call time, iterate `[UIApplication sharedApplication].connectedScenes` (guarded by iOS 13 availability) and call `setupSceneDelegateProxyForScene:` for each scene already connected (covers scenes connecting before ANE context init runs).
   - Register an observer for the system notification `UISceneWillConnectNotification` that calls `setupSceneDelegateProxyForScene:` for the newly connecting scene (covers scenes/windows created after ANE init, e.g. multi-window iPadOS) — relies on the `delegateClass`/KVO fallback from step 2 since the delegate may not be assigned yet (Critical Issue #2).
   - Keep the existing `application:` swizzle calls completely unchanged.

4. **Update `Core.m`** (*depends on step 3*): no changes needed beyond what already calls `setupAppDelegateProxy:` — the new scene logic lives inside that same entry point, so the existing single call site in `CoreContextInitializer` (~source/platform/ios/Core/Core.m#L101) automatically picks it up.

5. **Update `DTCAppDelegateProxy.h`**: no new public API needed (scene setup is invoked internally from `setupAppDelegateProxy:`), keeping the public interface unchanged for consumers.

6. ✅ **DONE — Fixed the `"optons"` typo in `DTNotifications.m`** (*independent, bundled in per recommendation*): `-openURLOptions:` now reads `@"options"` (was `@"optons"`) — every consuming extension's `openURL:options:` delegate callback previously silently received `nil` options due to this key mismatch. Corelibraries-side change; still need to **rebuild/bump `CoreNativeExtension.xcframework`** so consumers pick up the fix.

## Implementation Breakdown (remaining work, sequenced as independent commits/PRs)

Steps 1 (swizzle hardening) and the typo fix above are done. The remaining scene-support work (plan Steps 2–3) should land as these incremental, individually buildable/testable pieces rather than one large change:

1. **Rebuild/bump `CoreNativeExtension.xcframework`** for the typo fix alone and verify existing consumers pick up populated `options` dictionaries — no scene code involved, ship/verify this in isolation first.
2. **Scene discovery skeleton** — add `+setupSceneDelegateProxyForScene:` as a no-op/logging-only stub, plus the `connectedScenes` enumeration and `UISceneWillConnectNotification` observer inside `setupAppDelegateProxy:`. Verify via logging that scenes are discovered correctly and confirm whether Critical Issue #2 (nil delegate at connect time) actually reproduces on device/simulator before building the fallback.
3. **Delegate resolution fallback (Critical Issue #2)** — implement the `scene.session.configuration.delegateClass` fallback and KVO observer for late-bound delegates. Verify in isolation by logging which path resolved the delegate class across a few OS versions.
4. **Steady-state scene swizzles** — `scene:continueUserActivity:`, `scene:openURLContexts:`, `windowScene:performActionForShortcutItem:completionHandler:`. These mirror existing AppDelegate posting code almost 1:1 and are easiest to iterate on against a running (not cold-launched) test app.
5. **Cold-launch handling** — swizzle `scene:willConnectToSession:options:` and parse `connectionOptions` (URLContexts/userActivities/shortcutItem), taking care to avoid duplicate posts if UIKit also calls the steady-state methods on some OS versions. Do this last since it depends on steps 3–4 being solid, and requires the dedicated cold-start test harness (terminate app, launch via URL/Universal Link) from the Verification section.

Suggested grouping for review: (1) is a standalone prep commit; (2)–(3) form an "infrastructure" PR; (4)–(5) form the "feature" PR — keeping regressions easy to bisect.

## Relevant files

- `source/platform/ios/Core/DTCAppDelegateProxy.m` — harden `+swizzle:implementation:class:` (class_addMethod-first) and synchronize `originalMethods_`; add scene delegate swizzle functions (`DTCoreSceneWillConnectWithOptions`, `DTCoreSceneContinueUserActivity`, `DTCoreSceneOpenURLContexts`, `DTCoreScenePerformActionForShortcutItem`); extend `+setupAppDelegateProxy:` to enumerate/observe scenes with the `delegateClass`/KVO fallback for not-yet-assigned delegates.
- `source/platform/ios/Core/DTCAppDelegateProxy.h` — unchanged (no new public methods required).
- corelibraries `DTNotifications.m` — fix the `"optons"`→`"options"` key typo in `-openURLOptions:` (bundled per recommendation); notification names/payload shapes otherwise unchanged so no consumer-facing API changes.
- `DTNotifications` consumers in other extensions — **no changes required** beyond picking up the rebuilt `CoreNativeExtension` for the typo fix.

## Verification

1. Build the Core ANE (existing `build.xml`/`build_config` — new code lives in the same `.m` file already compiled, no build config changes needed) and confirm no compile warnings/errors for the new `API_AVAILABLE(ios(13.0))`-guarded code.
2. Regression test: run an existing AppDelegate-only test app (no `UIApplicationSceneManifest`) — `connectedScenes` will be empty, so new code paths are inert; confirm existing openURL/continueUserActivity/shortcut/notification flows still work exactly as before.
3. New scenario test: build a minimal native iOS host app (or manually add `UIApplicationSceneManifest` + a `UISceneDelegate`/`UIWindowSceneDelegate` subclass to a test AIR-packaged app's Info.plist/Xcode project) that links `CoreNativeExtension.framework` + the Core ANE, and verify:
   - Opening via a custom URL scheme dispatches `DTCoreOpenURLOptionsNotification` with correct `url`.
   - Universal link / handoff dispatches `DTCoreApplicationContinueUserActivityNotification`.
   - Long-press shortcut action dispatches `DTCorePerformActionForShortcutItemNotification`.
   - A downstream extension using `DTNotifications`/`DTNotificationsDelegate` receives these callbacks unmodified, and confirm the `options` dictionary is now populated (typo fix) instead of `nil`.
4. **Cold-start test harness** (explicitly covering Critical Issue #1): terminate the app fully, then cold-launch via (a) a custom URL scheme and (b) a Universal Link, confirming `connectionOptions` are correctly translated into `DTCoreOpenURLOptionsNotification` / `DTCoreApplicationContinueUserActivityNotification` from `scene:willConnectToSession:options:` — not just from `scene:openURLContexts:`/`scene:continueUserActivity:` on an already-running app.
5. Multi-window test (iPadOS): open two scenes simultaneously each triggered by a different URL and confirm two distinct `DTCoreOpenURLOptionsNotification` posts arrive with correct per-context URLs, with no crash/corruption from concurrent `originalMethods_` access (Critical Issue #5).
6. Confirm `didFinishLaunchingWithOptions` and push/local notification delegate flows are unaffected (they remain on the AppDelegate swizzle path, untouched by this change).

## Decisions

- Scope limited to mirroring only the AppDelegate methods currently swizzled that have direct Scene-based equivalents (`willConnectToSession`/cold-launch options, continueUserActivity, openURL, shortcut items). `sceneDidBecomeActive`/`sceneWillResignActive`/etc. are **not** added since Core doesn't currently proxy the AppDelegate active/resign-active equivalents either — kept out of scope for parity.
- No changes to Adobe AIR/adt packager — out of this repo's control; this change only makes the ANE resilient if/when a host app's Info.plist gains a scene manifest, requiring zero extension-author changes.
- Reusing existing notification names/payload shapes for the new scene-based posts avoids a breaking change across all consumer extensions; the `DTNotifications` public delegate protocol is unchanged.
- The `"optons"`/`"options"` key typo in `DTNotifications.m`'s `-openURLOptions:` **will be fixed as part of this change** (per recommendation), since it currently means `openURL:options:` delegate callbacks always receive `nil` options in every consuming extension — no reason to carry forward dead payload data while touching this area.

## Further Considerations

1. Since it's unclear if/when AIR will adopt scene lifecycle, consider whether to ship this defensively now (low risk, inert on non-scene apps) vs. wait until AIR confirms the change — recommend shipping now given the fix is a no-op for current apps and avoids being caught off guard.
2. Document for downstream extension authors that `DTCoreOpenURLOptionsNotification` can now legitimately fire multiple times in quick succession on multi-window devices (Critical Issue #4), so they should not assume a single global "open" event per app lifetime.
