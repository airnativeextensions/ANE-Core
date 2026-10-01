###### 2026.10.01 [v7.7.0]

```
feat(android,ios): add robust logging system with return path for native logs to air
feat(ios): fre utils cleanup, documentation
fix(ios): correct image utils transparency issue
feat(ios): expand utils to cover more cases
feat(ios): harden swizzling helper implementation - correct issue with openurl options
feat(android): embed consumer proguard rules in ane jar
```

###### 2025.10.10 [v7.6.0]

```
feat(android): updates to the android core libraries
```

###### 2025.09.24 [v7.5.5]

```
feat(macos): correct minimum macos version to 10.13
feat(ios): update to latest xcode
```

###### 2024.06.26 [v7.5.4]

```
feat(android): common permission helpers to better handle denied state
```

###### 2024.06.25 [v7.5.3]

```
feat(android): common permission helpers to better handle denied state
```

###### 2024.06.19 [v7.5.2]

```
fix(ios): correct a potential issue in the FRE image utilities
fix(android): correct handling of '%' characters and other potential faults in logging implementation
```

###### 2023.09.22 [v7.5.1]

```
fix(android): correct operation of authorisation result callbacks and add a complete event
```

###### 2023.08.08 [v7.5.0]

```
feat(android): add new IActivityResultExtensionContext to simplify handling startActivityForResult on various platforms
```

###### 2023.07.06 [v7.4.0]

```
feat(android): new IExtensionContext implementation to automatically create an activity for handling activity results
```

###### 2023.06.29 [v7.3.1]

```
feat(android): auth updates to ensure compatibility with unity and other frameworks
```

###### 2023.06.13 [v7.3.0]

```
feat(ios,macos,tvos): add some json utilities
```

###### 2023.02.15 [v7.2.0]

```
feat(macos): add support for apple silicon processors
fix(android,auth): correct issue with fallback authorisation implementation events being dispatched to wrong extension
```

###### 2023.01.23 [v7.0.2]

```
feat(android): add additional authorisation types for services that distinguish between always and in use
```

###### 2023.01.16 [v7.0.1]

```
feat(android): add additional authorisation type to return events for distinguishing request source
```

###### 2023.01.10 [v7.0.0]

```
feat(ios): remove bitcode 
feat(ios): add no-objc-msgsend-selector-stubs compiler flag
feat(android): add centralised permissions/auth functionality using new AIR callbacks
```

###### 2022.08.01 [v6.4.8]

```
fix(android): add some additional file provider utilities for handling cache files
```

###### 2022.08.01 [v6.4.7]

```
fix(android): add some additional file provider utilities for handling cache files
```

###### 2022.06.23 [v6.4.6]

```
fix(macos): correct format of frameworks included in macos build allowing symlinks
feat(integration): update public integration libraries
```

###### 2021.08.20 [v6.4.3]

```
AIR package release
```


###### 2021.05.18 [v6.4.001]

```
Added windows 64bit support
```


###### 2021.03.16 [v6.3.023]

```
Removed ios_version_min (#26)
```


###### 2020.12.07 [v6.3.022]

```
Corrected framework in macOS build which caused issues with signing
```


###### 2020.09.22 [v6.2.015]

```
Added additional iOS helper utilities
```


###### 2020.09.12 [v6.2.013]

```
Android x64 support
```


###### 2020.09.01 [v6.1.009]

```
Updated Core iOS libraries - fixed some iOS 12.4 related issues
```


###### 2020.08.19 [v6.0.006]

```
Updated macOS library
```


###### 2020.08.14 [v6.0.001]

```
Added macOS Core implementation for macOS extensions
```


###### 2020.06.02 [v5.1.346]

```
Fixed iOS issue with multiple dispatching of launch options (eg notification events)
```

###### 2020.05.15 [v5.1.343]

```
Updated to new iOS core library xcframework
Updated windows extcontext base code
```


###### 2020.04.23 [v5.0.338]

```
Windows bug fix for ADL crash and removed event log usage (resolves #19)
```


###### 2020.03.20 [v5.0.327]

```
Android X (resolves #18)
Added windows platform for common dependencies
```


###### 2019.11.25 [v4.0.280]

```
Updated iOS libraries
```


###### 2019.08.12 [v4.0.268]

```
Android 64bit support (resolves #8) 
Increased iOS minimum version to 9
```


###### 2019.02.22 [v3.3.252]

```
Updated minimum iOS version to 8.0
```


###### 2018.12.06 [v3.2.243]

```
Corrected issue with shortcut actions when in background
```


###### 2018.10.29 [v3.2.239]

```
Updated generic FileProvider asset utilities for Android
```


###### 2018.10.27 [v3.2.230]

```
Updated generic FileProvider utilities for Android
```


###### 2018.08.15 [v3.2.224]

```
Lowered minimum supported version to AIR 24
```


###### 2018.07.21 [v3.2.215]

```
Updated core Android lib
```


###### 2018.06.25 [v3.2.202]

```
Reduced swf-version to increase compatibility
```


###### 2018.06.24 [v3.2.198]

```
Android: Updated image conversion library
```


###### 2018.06.01 [v3.2.187]

```
Added functionality to help with @available usage in iOS frameworks
```


###### 2018.04.13 [v3.1.174]

```
Fixed issue with Android extensions accessing AIR view
```


###### 2018.04.11 [v3.1.170]

```
Additional Android helpers
```


###### 2018.04.04 [v3.1.166]

```
Updated internal caching of asset files
```


###### 2018.03.24 [v3.1.152]

```
Added additional Android support libs
```


###### 2018.03.10 [v3.1.133]

```
Updated iOS lib
```


###### 2018.02.22 [v3.1.129]

```
Latest updates to core libraries
```


###### 2018.02.09 [v3.0.117]

```
Added AppleTV platform and some bug fixes
```


###### 2017.11.21 [v2.6.084]

```
Updates for Android core libs
```


###### 2017.09.29 [v2.5.075]

```
Updates to the iOS App Delegate overrides to work better with other ANEs
```


###### 2017.09.29 [v2.5.073]

```
Updates to the iOS App Delegate overrides to work better with other ANEs
```


###### 2017.08.08 [v2.4.047]

```
Updated framework signature
```


###### 2017.07.10 [v2.3.006]

```
iOS 10: Centralisation of new notifications delegate
```


###### 2017.06.29 [v2.2.011]

```
Updates for Firebase Dynamic Links
```


###### 2017.04.13 [v2.1.020]

```
Updates to core libraries
```


###### 2017.04.06 [v2.1.018]

```
Updated Android libraries
```


###### 2017.01.12 [v2.1.010]

```
Android: removed some old permissions
```


###### 2017.01.12 [v2.1.008]

```
Small addition of an Android class used by some extensions
```


###### 2016.12.15 [v2.1.006]

```
Latest Core additions and added noair version for conflict resolutions
```


###### 2016.11.14 [v2.0.010]

```
Added additional Android core class
```


###### 2016.09.01 [v2.0.005]

```
Added some Android logging utilities
```


###### 2016.08.11 [v2.0.001]

```
Android support library update and new version system
```


######  2016.08.01

```
Updated core Android tools
```


######  2016.07.16

```
Updated to be compatible with latest support ANEs
```


######  2016.07.03

```
Added new common Android dependencies
```


######  2016.06.28

```
Update to include common AIR extension classes
```


###### 2016.04.01

```
Latest updates for new notifications and force touch features
```


###### 2015.06.15

```
Removed debug code from AS lib
iOS: Updated to latest common lib
Android: Windows: Fix for bug in AIR packager resulting in missing resources
Android: x86 Support
```


###### 2015.06.09

```
Changed core symbol definitions
```


###### 2015.05.22

```
iOS: Corrected InvokeEvent operation (resolves #2, resolves distriqt/ANE-PushNotifications/#32, resolves distriqt/ANE-FacebookAPI/#27)
```


###### 2015.03.04

```
Initial release
```


