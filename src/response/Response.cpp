#include "Response.hpp"
#include "CGI.hpp"
#include "../utils/log.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

// CONSTRUCTOR
Response::Response(class Request request, class Configuration config)
	: _request(request), _config(config)
{
	if (_request.getURI() == "/")
		_filepath = _config.getPathWebsite() +  "/index.html";
	else if (_request.getURI().rfind('.') == string::npos)
		_filepath = _config.getPathWebsite() + _request.getURI() + ".html";
	else
		_filepath = _config.getPathWebsite() + _request.getURI();

	initStatusCodes();
	initContentTypes();
}

// DESTRUCTOR
Response::~Response() { }

// PUBLIC FUNTIONS
string Response::getFilepath() { return _filepath; }

string Response::getMessage()
{
	_status = setStatus();

	if (_status == MOVED_PERMANENTLY || _status == FOUND)
		_content = redirect();
	else if (_status == 404)
		_content = fileNotFound();
	else if (_status != OK)
		_content = createErrorHTML();
	else if (_request.isCGI()) {
		class CGI CGI(_request, _config);
		_content = CGI.ExecuteCGI();
		return createResponse();
	}
	else
		_content = getFileContent();
	return createResponse();
}

// PRIVATE FUNCTIONS
void	Response::initStatusCodes()
{
	_status_codes[200] = "200 OK\n";
	// _status_codes[201] = "201 Created\n";
	// _status_codes[202] = "202 Accepted\n";
	// _status_codes[204] = "204 No Content\n";
	// _status_codes[300] = "300 Multiple Choice\n";
	_status_codes[301] = "301 Moved Permanently\n";
	_status_codes[302] = "302 Found\n";
	_status_codes[400] = "400 Bad Request\n";
	// _status_codes[401] = "401 Unauthorized\n";
	// _status_codes[403] = "403 Forbidden\n";
	_status_codes[404] = "404 Not Found\n";
	_status_codes[405] = "405 Method Not Allowed\n";
	// _status_codes[413] = "413 Request Entity Too Large\n";
	_status_codes[415] = "415 Unsupported Media Type\n";
	// _status_codes[500] = "500 Internal Server Error\n";
	_status_codes[501] = "501 Not Implemented\n";
	// _status_codes[502] = "502 Bad Gateway\n";
	// _status_codes[504] = "504 Gateway Timeout\n";
	_status_codes[505] = "505 HTTP Version Not Supported\n";
}

void	Response::initContentTypes() 
{
	_content_types["text"]	= "Content-Type: text/plain\n";
	_content_types[".txt"]	= "Content-Type: text/plain; charset=utf-8\n";
	_content_types[".html"] = "Content-Type: text/html; charset=utf-8\n";
	_content_types[".css"] 	= "Content-Type: text/css; charset=utf-8\n";
	_content_types[".jpg"] 	= "Content-type: image/jpg\n";
	_content_types[".jpeg"] = "Content-type: image/jpeg\n";
	_content_types[".png"] 	= "Content-type: image/png\n";
	_content_types[".mp4"] 	= "Content-type: video/mp4\n";
	_content_types[".ico"] 	= "Content-type: image/vnd.microsoft.icon\n";
	_content_types[".php"] 	= "Content-Type: text/plain; charset=utf-8\n";
	_content_types[".js"] 	= "Content-Type: application/javascript\n";
	_content_types[".gif"] 	= "Content-Type: image/gif\n";
}
#include <unistd.h>
int		Response::setStatus()
{
	if (_request.getStatus() != OK)
		return _request.getStatus();

	if (_request.isCGI())
		return OK;

	basic_ifstream<char> input_stream(_filepath.c_str());
	if (input_stream.is_open()) {
		input_stream.close();
		return OK;
	}

	if (_request.getURI() == "/permanently_moved")
		return MOVED_PERMANENTLY;

	if (_request.getURI() == "/temporary_unavailable")
		return FOUND;
	if (_filepath.rfind('/') > _config.getPathWebsite().length()) {
		const char * dir = _filepath.substr(0, _filepath.rfind('/')).c_str();
		struct stat sb;
		if (stat(dir, &sb) == 0)
			return FOUND;
	}

	if(_content_types.find(_request.getExtension()) == _content_types.end())
		return UNSUPPORTED_MEDIA_TYPE;

	return NOT_FOUND;
}

string Response::redirect()
{
	// TO DO: costum redirect pages
	// TO DO: error codes from 301 not correctly displayed on terminal
	return createErrorHTML();
}

string Response::fileNotFound()
{
	_status = 404;

	// TO DO: config file should check if supplied path exists
	if (_config.get404() != "default") {
		_filepath = _config.getPathWebsite() + _config.get404();
		return getFileContent();
	}
	return createErrorHTML();
}

string Response::createErrorHTML()
{
	string meta  = "";
	if (_status == MOVED_PERMANENTLY || _status == FOUND)
		meta = "<meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"5; url=/\"/>";

	ostringstream ss;
	ss	<< "<!DOCTYPE html><html lang=\"en\"><head><title>"
		<< _status_codes[_status]
		<< "</title>"
		<< meta
		<< "</head><body><center><h1>"
		<< _status_codes[_status]
		<< "</h1></center></body></html>";
	return ss.str();
}

string Response::getFileContent()
{
	ifstream input_stream(_filepath.c_str());

	ostringstream ss;
	string line;
	while (getline(input_stream, line))
		ss << line << endl;
	return ss.str();
}

string Response::createResponse()
{
	ostringstream ss;
	ss	<< _request.getHTTPVersion() << " "
		<< _status_codes[_status]
		<< _content_types[ _request.getExtension()]
		<< "Content-Length: "
		<< _content.size() << "\n\n"
		<< _content;
	return ss.str();
}
