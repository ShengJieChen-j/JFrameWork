#include "stdafx.h"
#include "StringUtil.h"
#include <locale.h>  

namespace base
{ 
	std::string StringUtil::lTrim(std::string& str)
	{
		const std::string drop = " \t";
		return str.erase(0, str.find_first_not_of(drop));
	}

	std::string StringUtil::rTrim(std::string& str)
	{
		const std::string drop = " \t";
		return str.erase(str.find_last_not_of(drop) + 1);
	}

	std::string StringUtil::trim(std::string& str)
	{
		const std::string drop = " \t";
		str.erase(str.find_last_not_of(drop) + 1);
		return str.erase(0, str.find_first_not_of(drop));
	}

	std::vector<std::string> StringUtil::split(const std::string& str, const char delim)
	{
		std::vector<std::string> result;
		size_t start = 0;
		size_t pos = str.find(delim);
		while (pos != std::string::npos)
		{
			if (pos > start)
			{
				result.push_back(str.substr(start, pos - start));
			}
			start = pos + 1;
			pos = str.find(delim, start);
		}

		if (start < str.length())
		{
			result.push_back(str.substr(start));
		}

		return result;
	}
}