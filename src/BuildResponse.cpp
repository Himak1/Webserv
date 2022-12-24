#include "BuildResponse.hpp"
#include "CGI.hpp"
#include "utils.hpp"

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
	if (_request.getMethod() == "POST") {
		class CGI CGI(_request, _config);
		std::string cgi = CGI.ExecuteCGI();
		return (createResponse(status, cgi, cgi.size()));
	}

	std::string content = readStream(_filename);
	if (content == "fileNotFound")
		return fileNotFound();

	return createResponse(status, content, getStreamSize(_filename));
}

// PRIVATE FUNCTIONS
std::string BuildResponse::fileNotFound() const
{
	std::string status = "404 Not Found";
	std::string content;
	if (_config.get404() != "default") {
		std::string filename = _config.getPathWebsite() + _config.get404();
		content = readStream(filename);
	}
	else {
		content = "<!DOCTYPE html>" \
				  "<html lang=\"en\">" \
				  "<head><title>404 Not Found</title></head>" \
				  "<body><center><h1>404 Not Found</h1></center>" \
				  "</body></html>";
	}
	return createResponse(status, content, content.size());
}

std::string BuildResponse::createResponse(
	std::string status, std::string content, int size) const
{
	std::string content_type = "\nContent-Type: text/html\nContent-Length: ";
	std::ostringstream ss;
	ss	<<  _request.getHTTPVersion() << " "
		<< status
		<< content_type
		<< size << "\n\n"
		<< content;
	return ss.str();
}
