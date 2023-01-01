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
	else if (_request.getURI().rfind('.') == std::string::npos)
		_filename = _config.getPathWebsite() + _request.getURI() + ".html";
	else
		_filename = _config.getPathWebsite() + _request.getURI();

	_content_types = {
		{ "text",	"Content-Type: text/plain\n" },
		{ ".txt",	"Content-Type: text/plain; charset=utf-8\n" },
		{ ".html",	"Content-Type: text/html; charset=utf-8\n" },
		{ ".css",	"Content-Type: text/css; charset=utf-8\n" },
		{ ".jpg",	"Content-type: image/jpg\n" },
		{ ".jpeg",	"Content-type: image/jpeg\n" },
		{ ".png",	"Content-type: image/png\n" },
		{ ".mp4",	"Content-type: video/mp4\n" },
		{ ".ico",	"Content-type: image/vnd.microsoft.icon\n" },
		{ ".php",	"Content-Type: text/plain; charset=utf-8\n" },
		{ ".js",	"Content-Type: application/javascript\n" },
		{ ".gif",	"Content-Type: image/gif\n" }
	};

	// std::cout << _filename << std::endl;
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

std::string Response::createResponse()
{
	std::string extension = _request.getExtension();

	if(_content_types.find(extension) == _content_types.end())
		return fileNotFound();

	std::ostringstream ss;
	ss	<<  _request.getHTTPVersion() << " "
		<< _status
		<< "\n"
		<< _content_types[extension]
		<< "Content-Length: "
		<< _content.size() << "\n\n"
		<< _content;
	return ss.str();
}
