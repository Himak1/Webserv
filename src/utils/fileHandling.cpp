#include "fileHandling.hpp"

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

void	writeStringToFile(string file_data, string upload_path) 
{
	ofstream fout(upload_path);
	fout << file_data << endl;
    fout.close();
}
