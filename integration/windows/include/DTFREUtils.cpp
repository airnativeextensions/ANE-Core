#include "DTFREUtils.h"

#include <vcclr.h>
#include <windows.h>


static std::string Utf8FromString(System::String^ str)
{
	if (str == nullptr) return std::string();

	pin_ptr<const wchar_t> wch = PtrToStringChars(str);

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wch, -1, nullptr, 0, nullptr, nullptr);
	if (sizeNeeded <= 0) return std::string();

	std::string utf8(sizeNeeded, '\0');
	int written = WideCharToMultiByte(CP_UTF8, 0, wch, -1, &utf8[0], sizeNeeded, nullptr, nullptr);
	if (written <= 0) return std::string();
	utf8.resize(written - 1);
	return utf8;
}


FREObject DTFREUtils::newFREObjectFromBool(bool value)
{
	FREObject result;
	if (FRE_OK == FRENewObjectFromBool(value, &result))
	{
		return result;
	}
	return NULL;
}


FREObject DTFREUtils::newFREObjectFromInt(int value)
{
	FREObject result;
	if (FRE_OK == FRENewObjectFromInt32(value, &result))
	{
		return result;
	}
	return NULL;
}


FREObject DTFREUtils::newFREObjectFromString(System::String^ value)
{
	return newFREObjectFromStdString(Utf8FromString(value));
}


FREObject DTFREUtils::newFREObjectFromStdString(const std::string& value)
{
	FREObject result;
	if (FRE_OK == FRENewObjectFromUTF8((uint32_t)value.length(), (const uint8_t*)value.c_str(), &result))
	{
		return result;
	}
	return NULL;
}


int DTFREUtils::getFREObjectAsInt(FREObject object)
{
	int32_t value;
	if (FRE_OK == FREGetObjectAsInt32(object, &value))
	{
		return value;
	}
	return 0;
}


bool DTFREUtils::getFREObjectAsBool(FREObject object)
{
	uint32_t value;
	if (FRE_OK == FREGetObjectAsBool(object, &value))
	{
		return value != 0;
	}
	return false;
}


System::String^ DTFREUtils::getFREObjectAsString(FREObject object)
{
	uint32_t stringLength;
	const uint8_t* stringPtr;
	if (FRE_OK == FREGetObjectAsUTF8(object, &stringLength, &stringPtr))
	{
		return System::Text::Encoding::UTF8->GetString(const_cast<uint8_t*>(stringPtr), (int)stringLength);
	}
	return System::String::Empty;
}


std::string DTFREUtils::getFREObjectAsStdString(FREObject object)
{
	uint32_t stringLength;
	const uint8_t* stringPtr;
	if (FRE_OK == FREGetObjectAsUTF8(object, &stringLength, &stringPtr))
	{
		return std::string(stringPtr, stringPtr + stringLength);
	}
	return std::string();
}


int DTFREUtils::getFREObjectPropertyAsInt(FREObject object, const char* property)
{
	FREObject propertyObject;
	if (FRE_OK == FREGetObjectProperty(object, (const uint8_t*)property, &propertyObject, NULL))
	{
		return getFREObjectAsInt(propertyObject);
	}
	return 0;
}


System::String^ DTFREUtils::getFREObjectPropertyAsString(FREObject object, const char* property)
{
	FREObject propertyObject;
	if (FRE_OK == FREGetObjectProperty(object, (const uint8_t*)property, &propertyObject, NULL))
	{
		return getFREObjectAsString(propertyObject);
	}
	return System::String::Empty;
}


std::string DTFREUtils::getFREObjectPropertyAsStdString(FREObject object, const char* property)
{
	FREObject propertyObject;
	if (FRE_OK == FREGetObjectProperty(object, (const uint8_t*)property, &propertyObject, NULL))
	{
		return getFREObjectAsStdString(propertyObject);
	}
	return std::string();
}


System::Collections::ArrayList^ DTFREUtils::stringArrayFromFREArray(FREObject arrayObject)
{
	uint32_t length;
	System::Collections::ArrayList^ ret = gcnew System::Collections::ArrayList();
	if (FRE_OK == FREGetArrayLength(arrayObject, &length))
	{
		for (uint32_t i = 0; i < length; i++)
		{
			FREObject stringObject;
			if (FRE_OK == FREGetArrayElementAt(arrayObject, i, &stringObject))
			{
				ret->Add(getFREObjectAsString(stringObject));
			}
		}
	}
	return ret;
}
