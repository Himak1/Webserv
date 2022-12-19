#include <BuildResponse.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

// CONSTRUCTOR
BuildResponse::BuildResponse(std::string uri) : _uri(uri) {
	if (_uri == "/")
		_filename = "www/index.html";
	else if (_uri.find(".html", _uri.length() - 5) != std::string::npos)
		_filename =  "www" + _uri;
	else
		_filename =  "www" + _uri + ".html";
}

// DESTRUCTOR
BuildResponse::~BuildResponse() { }

// PUBLIC FUNTIONS
std::string BuildResponse::getMessage()
{
	if (_uri == "/favicon.ico")
		return "favicon";

	std::ifstream htmlFile(_filename);
	if (!htmlFile.is_open())
		return (fileNotFound());

	std::ostringstream ss;
	ss	<< "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " 
		<< getFileSize(_filename) << "\n\n";

	std::string line;
	while (std::getline(htmlFile, line))
		ss << line << std::endl;

	htmlFile.close();
	return ss.str();
}

// PRIVATE FUNCTIONS
int BuildResponse::getFileSize(std::string filename)
{
	FILE *p_file = NULL;
	p_file = fopen(filename.c_str(),"rb");
	fseek(p_file, 0, SEEK_END);
	int size = ftell(p_file);
	fclose(p_file);
	return size;
}

std::string BuildResponse::fileNotFound()
{
	std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> 404 </h1><p> File not found </p></body></html>";
	std::ostringstream ss;

	ss	<< "HTTP/1.1 404 OK\nContent-Type: text/html\nContent-Length: " 
		<< htmlFile.size() << "\n\n"
		<< htmlFile;

	return ss.str();
}
