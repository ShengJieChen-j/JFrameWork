/******************************************************************
	创建日期：  2021年8月1日
	文件名称：  StringUtil.h
	说    明：

	当前版本：  1.00
	作    者：  huamogu
	概    述：  

*******************************************************************/
#pragma once

#include <vector>
#include <string>

namespace base
{
	class StringUtil
	{
	public:
		static std::string trim(std::string& str);
		static std::string lTrim(std::string& str);
		static std::string rTrim(std::string& str);
		static std::vector<std::string> split(const std::string& str, const char delim);

		static std::wstring s2ws(const std::string& s);
		static std::wstring s2ws(const char* s);
		static std::string ws2s(const wchar_t* ws);
		static std::string ws2s(const std::wstring& ws);
	};
}