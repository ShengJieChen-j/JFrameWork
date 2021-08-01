/******************************************************************
	�������ڣ�  2021��8��1��
	�ļ����ƣ�  StringUtil.h
	˵    ����

	��ǰ�汾��  1.00
	��    �ߣ�  huamogu
	��    ����  

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