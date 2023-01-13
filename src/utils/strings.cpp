#include "strings.hpp"

string safe_substr(string str, int start, int length)
{
	if (start < 0)
		start = 0;
	if (str.length() <= start)
		return (str);

	if (length < 0)
		length = str.length() - start;
	if (str.length() < (start + length))
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
