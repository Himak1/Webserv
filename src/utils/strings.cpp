#include "strings.hpp"
#include <sstream>

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

size_t	ft_strlen(char *s)
{
	size_t	i;

	for (i = 0; s[i]; i++) ;
	return i;
}

std::string number_to_string (unsigned int number)
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}
