#pragma once

// Resolves managed assemblies (e.g. CoreNativeExtension.dll, FREInterface.dll) packaged in other
// extensions, preferring the com.distriqt.Core extension. Requires /clr.
//
// Call DTAssemblyResolver::registerResolver() in the extension initializer, before any
// function that uses types from those assemblies is called.

namespace DTAssemblyResolver
{
	using namespace System;
	using namespace System::Collections::Generic;
	using namespace System::IO;
	using namespace System::Reflection;

	ref class Resolver abstract sealed
	{
	public:
		literal String^ CORE_EXTENSION_ID = "com.distriqt.Core";


		static Assembly^ resolve(Object^ sender, ResolveEventArgs^ args)
		{
			try
			{
				String^ name = (gcnew AssemblyName(args->Name))->Name;

				// Assemblies loaded by LoadFile aren't visible to the binder, so reuse any existing copy
				Assembly^ loaded = findLoaded(name);
				if (loaded != nullptr) return loaded;

				String^ dllName = name + ".dll";
				for each (String^ root in extensionRoots())
				{
					if (!Directory::Exists(root)) continue;

					array<String^>^ directories = Directory::GetDirectories(root);
					for each (String^ dir in directories)
					{
						if (!isCoreExtensionDirectory(dir)) continue;
						Assembly^ assembly = loadFromExtension(dir, dllName);
						if (assembly != nullptr) return assembly;
					}
					for each (String^ dir in directories)
					{
						if (isCoreExtensionDirectory(dir)) continue;
						Assembly^ assembly = loadFromExtension(dir, dllName);
						if (assembly != nullptr) return assembly;
					}
				}
			}
			catch (Exception^ e)
			{
				Diagnostics::Trace::WriteLine("DTAssemblyResolver::resolve(): " + e->Message);
			}
			return nullptr;
		}


	private:
		static Assembly^ findLoaded(String^ name)
		{
			for each (Assembly^ assembly in AppDomain::CurrentDomain->GetAssemblies())
			{
				try
				{
					if (String::Equals(assembly->GetName()->Name, name, StringComparison::OrdinalIgnoreCase))
						return assembly;
				}
				catch (Exception^)
				{
				}
			}
			return nullptr;
		}


		// ADL: each -extdir argument, packaged app: <app>\META-INF\AIR\extensions
		static List<String^>^ extensionRoots()
		{
			List<String^>^ roots = gcnew List<String^>();
			array<String^>^ args = Environment::GetCommandLineArgs();
			for (int i = 0; i < args->Length - 1; i++)
			{
				if (args[i]->Equals("-extdir"))
				{
					roots->Add(Path::GetFullPath(args[i + 1]->Replace("/", "\\")));
				}
			}
			if (roots->Count == 0)
			{
				String^ exePath = Diagnostics::Process::GetCurrentProcess()->MainModule->FileName;
				roots->Add(Path::Combine(Path::GetDirectoryName(exePath), "META-INF\\AIR\\extensions"));
			}
			return roots;
		}


		// Packaged apps use the extension id, ADL extension directories are commonly named <id>.ane
		static bool isCoreExtensionDirectory(String^ dir)
		{
			String^ name = Path::GetFileName(dir);
			return name->Equals(CORE_EXTENSION_ID, StringComparison::OrdinalIgnoreCase)
				|| name->Equals(CORE_EXTENSION_ID + ".ane", StringComparison::OrdinalIgnoreCase);
		}


		static Assembly^ loadFromExtension(String^ extensionDir, String^ dllName)
		{
			String^ platform = Environment::Is64BitProcess ? "x86-64" : "x86";
			String^ fileName = Path::Combine(extensionDir, "META-INF\\ANE\\Windows-" + platform + "\\" + dllName);
			if (!File::Exists(fileName)) return nullptr;
			try
			{
				return Assembly::LoadFile(fileName);
			}
			catch (Exception^ e)
			{
				Diagnostics::Trace::WriteLine("DTAssemblyResolver::loadFromExtension(): " + fileName + ": " + e->Message);
			}
			return nullptr;
		}
	};


	inline void registerResolver()
	{
		AppDomain::CurrentDomain->AssemblyResolve += gcnew ResolveEventHandler(&Resolver::resolve);
	}
}
