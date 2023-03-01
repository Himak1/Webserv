#include "fileHandling.hpp"
#include "strings.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

bool	isExistingFile(string filename)
{
	basic_ifstream<char> input_stream(filename.c_str());
	if (input_stream.is_open()) {
		input_stream.close();
		return true;
	}
	return false;
}

string	streamFileDataToString(string input_path)
{
	ifstream input_stream(input_path.c_str());
	ostringstream ss;
	string line;
	while (getline(input_stream, line))
		ss << line << endl;
	return ss.str();
}

string	streamPhpFileDataToString(string input_path)
{
	ifstream input_stream(input_path.c_str());
	ostringstream ss;
	string line;
	while (getline(input_stream, line))
		if (line.find("//") == string::npos)
			ss << line << endl;
	return ss.str();
}

void	writeStringToFile(string file_data, string upload_path) 
{
	ofstream fout(upload_path.c_str());
	fout << file_data << endl;
    fout.close();
}

string parseExtension(string uri)
{
	if (uri.find('?') != string::npos)
		uri = safe_substr(uri, 0, uri.find('?'));

	size_t extension_start = uri.rfind('.');
	if (extension_start != string::npos)
		return safe_substr(uri, extension_start, uri.length());
	return "";
}
