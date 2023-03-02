#ifndef FILE_HANDLING_H
# define FILE_HANDLING_H

#include <iostream>

using namespace std;

bool	isExistingFile(string filename);
string	streamFileDataToString(string input_path);
string	streamPhpFileDataToString(string input_path);
void	writeStringToFile(string file_data, string upload_path);
string  parseExtension(string uri);

#endif
