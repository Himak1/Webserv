#include "Response.hpp"
#include "CGI.hpp"
#include "../utils/utils.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

// CONSTRUCTOR
Response::Response(class Request request, class Configuration config)
	: _request(request), _config(config) {
	_status = "200 OK";

	if (_request.getURI() == "/")
		_filename = _config.getPathWebsite() +  "/index.html";
	else if (_request.getURI().find(".css", _request.getURI().length() - 4) != std::string::npos)
		_filename = _config.getPathWebsite() + _request.getURI();
	else if (_request.getURI() == "/favicon.ico")
		_filename = _config.getPathWebsite() + _request.getURI();
	else if (_request.getURI().find(".html", _request.getURI().length() - 5) != std::string::npos)
		_filename = _config.getPathWebsite() + _request.getURI();
	else
		_filename = _config.getPathWebsite() + _request.getURI() + ".html";

	std::cout << _filename << std::endl;
}

// DESTRUCTOR
Response::~Response() { }

// PUBLIC FUNTIONS
std::string Response::getFilename() { return _filename; }

std::string Response::getMessage()
{
	if (_request.getMethod() == "POST") {
		class CGI CGI(_request, _config);
		_content = CGI.ExecuteCGI();
		return createResponse();
	}

	_content = readStream(_filename);
	if (_content == "fileNotFound")
		return fileNotFound();

	return createResponse();
}

// PRIVATE FUNCTIONS
std::string Response::fileNotFound()
{
	_status = "404 Not Found";
	std::string content;
	if (_config.get404() != "default") {
		std::string filename = _config.getPathWebsite() + _config.get404();
		_content = readStream(filename);
	}
	else {
		_content = "<!DOCTYPE html>" \
				  "<html lang=\"en\">" \
				  "<head><title>404 Not Found</title></head>" \
				  "<body><center><h1>404 Not Found</h1></center>" \
				  "</body></html>";
	}
	return createResponse();
}

std::string Response::createResponse() const
{
	std::string content_type;
	if (_filename.find(".css", _filename.length() - 4) != std::string::npos)
		content_type = "Content-Type: text/css; charset=utf-8\nContent-Length: ";

	content_type = "\nContent-Type: text/html\nContent-Length: ";
	std::ostringstream ss;
	ss	<<  _request.getHTTPVersion() << " "
		<< _status
		<< content_type
		<< _content.size() << "\n\n"
		<< _content;
	return ss.str();
}
