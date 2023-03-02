#include "strings.hpp"
#include <sstream>
#include <iostream>
#include <string>

string safe_substr(string str, int start, int length)
{
	if (start < 0)
		start = 0;
	if (str.length() <= (unsigned long)start)
		return (str);

	if (length < 0)
		length = str.length() - start;
	if (str.length() < (unsigned long)(start + length))
		length = str.length() - start;

	return (str.substr(start, length));
}

string trim_spaces(const string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == string::npos)
		return str;
	size_t last = str.find_last_not_of(' ');
	return safe_substr(str, first, (last - first + 1));
}

string go_one_directory_up(string str)
{
	size_t length = str.rfind("/");
	if (length == 0 || length == string::npos)
		return "/";
	return safe_substr(str, 0, length);
}

string convertToString (size_t Number)
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}
