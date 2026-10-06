#pragma once

#include "FlashRuntimeExtensions.h"
#include <string>

// Requires /clr
class DTFREUtils
{

public:
	static FREObject newFREObjectFromBool(bool value);
	static FREObject newFREObjectFromInt(int value);
	static FREObject newFREObjectFromString(System::String^ value);
	static FREObject newFREObjectFromStdString(const std::string& value);


	static int getFREObjectAsInt(FREObject object);
	static bool getFREObjectAsBool(FREObject object);
	static System::String^ getFREObjectAsString(FREObject object);
	static std::string getFREObjectAsStdString(FREObject object);


	static int getFREObjectPropertyAsInt(FREObject object, const char* property);
	static System::String^ getFREObjectPropertyAsString(FREObject object, const char* property);
	static std::string getFREObjectPropertyAsStdString(FREObject object, const char* property);


	static System::Collections::ArrayList^ stringArrayFromFREArray(FREObject arrayObject);

};
