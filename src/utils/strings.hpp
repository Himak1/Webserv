#ifndef STRINGS_HPP
# define STRINGS_HPP

# include <string>

using namespace std;

string	safe_substr(string str, int start, int length);
string	trim_spaces(const string& str);
string	go_one_directory_up(string str);
string	convertToString(size_t Number);

#endif
