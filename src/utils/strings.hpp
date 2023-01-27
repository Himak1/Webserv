#ifndef STRINGS_HPP
# define STRINGS_HPP

# include <string>

using namespace std;

string safe_substr(string str, int start, int length);
string trim_spaces(const string& str);
size_t	ft_strlen(char *s);

#endif
