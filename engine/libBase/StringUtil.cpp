#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void lTrim(string& str)
{
	string::iterator   p = find_if_not(str.begin(), str.end(), std::function<int(char)>(isspace));
	str.erase(str.begin(), p);
}

void rTrim(string& str)
{
	string::reverse_iterator  p = find_if_not(str.rbegin(), str.rend(), std::function<int(char)>(isspace));
	str.erase(p.base(), str.end());
}

void trim(string& str)
{
	rTrim(str);
	lTrim(str);
}

void split(const string& s, vector<string>& tokens, const string& delimiters = " ")
{

	string::size_type lPos, rPos = 0;
	lPos = s.find_first_not_of(delimiters, rPos);
	rPos = s.find_first_of(delimiters, lPos);
	while (string::npos != lPos || string::npos != rPos)
	{
		tokens.push_back(s.substr(lPos, rPos - lPos));
		lPos = s.find_first_not_of(delimiters, rPos);
		rPos = s.find_first_of(delimiters, lPos);
	}
}