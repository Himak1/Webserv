#include "BuildResponse.hpp"
#include "CGI.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

// CONSTRUCTOR
BuildResponse::BuildResponse(class HTTPRequest request, class Configuration config)
	: _request(request), _config(config) {
	if (_request.getURI() == "/")
		_filename = _config.getPathWebsite() +  "/index.html";
	else if (_request.getURI() == "/favicon.ico")
		_filename = _config.getPathWebsite() + _request.getURI();
	else if (_request.getURI().find(".html", _request.getURI().length() - 5) != std::string::npos)
		_filename = _config.getPathWebsite() + _request.getURI();
	else
		_filename = _config.getPathWebsite() + _request.getURI() + ".html";
	
	// std::cout << _filename << std::endl;
}

// DESTRUCTOR
BuildResponse::~BuildResponse() { }

// PUBLIC FUNTIONS
std::string BuildResponse::getMessage(std::string status)
{
	std::basic_ifstream<char> input_stream(_filename);
	if (!input_stream.is_open()) {
		input_stream.close();
		return (fileNotFound());
	}

	input_stream.seekg(0, std::ios::end);
	std::streampos size = input_stream.tellg();
	input_stream.seekg(0, std::ios::beg);

	std::ostringstream ss;
	ss	<< streamStatus(status)
		<< size << "\n\n";

	std::string line;
	while (std::getline(input_stream, line))
		ss << line << std::endl;

	input_stream.close();

	class CGI CGI(_request, _config);
	CGI.ExecuteCGI();

	return ss.str();
}

// PRIVATE FUNCTIONS
std::string BuildResponse::fileNotFound()
{
	std::string status = "404 Not Found";
	if (_config.get404() != "default") {
		_filename = _config.getPathWebsite() + _config.get404();
		return BuildResponse::getMessage(status);
	}

	std::string htmlFile = "<!DOCTYPE html>" \
							"<html lang=\"en\">" \
							"<head><title>404 Not Found</title></head>" \
							"<body><center><h1>404 Not Found</h1></center>" \
							"</body></html>";
	std::ostringstream ss;
	ss	<< streamStatus(status)
		<< htmlFile.size() << "\n\n"
		<< htmlFile;

	return ss.str();
}

std::string BuildResponse::streamStatus(std::string status) const
{
	return _request.getHTTPVersion() + " " + status \
			+ "\nContent-Type: text/html\nContent-Length: ";
}
