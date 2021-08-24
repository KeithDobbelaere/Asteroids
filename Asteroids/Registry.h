#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef UNICODE
#undef UNICODE
#endif

#include <windows.h>
#include <iostream>
#include <optional>


using std::cout; using std::endl;

namespace Registry
{
	class RegistryError
		: public std::runtime_error
	{
	public:
		RegistryError(const char* message, LONG errorCode)
			: std::runtime_error{ message }
			, m_errorCode{ errorCode }
		{}

		LONG ErrorCode() const noexcept { return m_errorCode; }
	private:
		LONG m_errorCode;
	};

	HKEY AccessKey(HKEY hRootKey, const std::string& strKey)
	{
		HKEY hKey;
		LONG nError = RegOpenKeyEx(
			hRootKey,
			strKey.c_str(),
			NULL,
			KEY_ALL_ACCESS,
			&hKey
		);

		if (nError == ERROR_FILE_NOT_FOUND)
		{
			cout << "Creating registry key: " << strKey << endl;
			nError = RegCreateKeyEx(
				hRootKey,
				strKey.c_str(),
				NULL,
				nullptr,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				nullptr,
				&hKey,
				nullptr
			);
		}

		if (nError)
			throw RegistryError{ (std::string("Could not find or create ") + strKey).c_str() , nError };
		return hKey;
	}

	void SetStrVal(HKEY hKey, const std::string& lpValue, const std::string& data)
	{
		LONG nError = RegSetValueEx(hKey, lpValue.c_str(), NULL, REG_SZ, (LPBYTE)data.c_str(), data.size()+1);

		if (nError)
			throw RegistryError{ (std::string("Could not set registry value: ") + lpValue).c_str() , nError };
	}

	std::optional<std::string> GetStrVal(HKEY hKey, const std::string& subKey, const std::string& lpValue)
	{
		DWORD dataSize = {};
		LONG nError = RegGetValue(
			hKey,
			subKey.c_str(),
			lpValue.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&dataSize
		);

		if (nError == ERROR_FILE_NOT_FOUND)
			return std::nullopt;
		else if (nError != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read string from registry.", nError };

		std::string data;
		data.resize(dataSize);
		nError = RegGetValue(
			hKey,
			subKey.c_str(),
			lpValue.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			&data[0],
			&dataSize
		);
		if (nError != ERROR_SUCCESS)
		{
			throw RegistryError{ "Cannot read string from registry.", nError };
		}
		data.resize(dataSize - 1); // Exclude the NUL written by the Win32 API
		return data;
	}

	void SetIntVal(HKEY hKey, const std::string& lpValue, DWORD data)
	{
		LONG nError = RegSetValueEx(hKey, lpValue.c_str(), NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		if (nError)
			throw RegistryError{ (std::string("Could not set registry value: ") + lpValue).c_str() , nError };
	}

	std::optional<DWORD> GetIntVal(HKEY hKey, const std::string& subKey, const std::string& lpValue)
	{
		DWORD data{};		
		DWORD dataSize = sizeof(data);
		LONG nError = RegGetValue(
			hKey,
			subKey.c_str(),
			lpValue.c_str(),
			RRF_RT_REG_DWORD,
			nullptr,
			&data,
			&dataSize
		);

		if (nError == ERROR_FILE_NOT_FOUND)
			return std::nullopt;
		else if (nError != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read DWORD from registry.", nError };

		return data;
	}
}

//EXAMPLE:
//int main()
//{
//	DWORD v1 = 5;
//	std::string v2 = "Five";
//
//	std::string subKey = "SOFTWARE\\MyCompany";
//	HKEY hKey = Registry::AccessKey(HKEY_LOCAL_MACHINE, subKey);
//
//	Registry::SetIntVal(hKey, "Value1", v1);
//	Registry::SetStrVal(hKey, "Value2", v2);
//
//	RegCloseKey(hKey); //Close key after accessing it!
//
//	std::optional<DWORD> v3;
//	std::optional<std::string> v4;
//
//	v3 = Registry::GetIntVal(HKEY_LOCAL_MACHINE, subKey, "Value1");
//	v4 = Registry::GetStrVal(HKEY_LOCAL_MACHINE, subKey, "Value2");
//
//	return 0;
//}