#include "Response.hpp"
#include "CGI.hpp"
#include "../utils/log.hpp"
#include "../utils/strings.hpp"
#include "../utils/fileHandling.hpp"
#include "../configure/Location.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>

#include <list>

// CONSTRUCTOR
Response::Response(class Request request, class Configuration config)
	: _request(request), _config(config), _location()
{
	initStatusCodes();
	initContentTypes();
	setFilePath();
	setLocation();
	_status = setStatus();
	_content = getContent();
}

// DESTRUCTOR
Response::~Response() { }

// PUBLIC FUNTIONS
string 	Response::getFilepath() { return _filepath; }

string 	Response::getMessage()
{
	uploadFile();

	ostringstream ss;
	ss	<< _request.getHTTPVersion() << " "
		<< _status_codes[_status]
		<< _content_types[ _request.getExtension()]
		<< setCookie()
		<< "Content-Length: "
		<< _content.size() << "\n\n"
		<< _content;
	return ss.str();
}

// PRIVATE FUNCTIONS
void	Response::setFilePath()
{
	bool is_undefined_extension = _request.getMethod() != "DELETE"
									&& _request.getURI().rfind('.') == string::npos;

	if (_request.getURI() == "/" || is_undefined_extension) {
		list<string>::iterator it = _config.indexFiles.begin();
		cout << "SETPATHFILE |" << *it << endl;
		while (it != _config.indexFiles.end()) {
			cout << "SETPATHFILE |" << *it << endl;
			_filepath = _config.getRoot() + _request.getURI() + "/" + *it;
			if (isExistingFile(_filepath))
				return;
			++it;
		}
		if (it == _config.indexFiles.end())
			_status = NOT_FOUND;
	}
	_filepath = _config.getRoot() + _request.getURI();
}

void	Response::setLocation()
{
	string target = _request.getURI();
	list<Location*>::iterator it = findConfigLocation(target);
	if (it == _config.locations.end()) {
		_status = INTERNAL_SERVER_ERROR;
	}
	_location = (*it);
}

void	Response::initStatusCodes()
{
	_status_codes[200] = "200 OK\n";
	_status_codes[202] = "202 Accepted\n";
	_status_codes[204] = "204 No Content\n";
	_status_codes[301] = "301 Moved Permanently\n";
	_status_codes[302] = "302 Found\n";
	_status_codes[400] = "400 Bad Request\n";
	_status_codes[404] = "404 Not Found\n";
	_status_codes[405] = "405 Method Not Allowed\n";
	_status_codes[413] = "413 Request Entity Too Large\n";
	_status_codes[415] = "415 Unsupported Media Type\n";
	_status_codes[500] = "500 Internal Server Error\n";
	_status_codes[501] = "501 Not Implemented\n";
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
	_content_types[".php"] 	= "Content-Type: text/html; charset=utf-8\n";
	_content_types[".js"] 	= "Content-Type: application/javascript\n";
	_content_types[".gif"] 	= "Content-Type: image/gif\n";
}

int		Response::setStatus()
{
	bool is_correct_HTTP		= _request.getHTTPVersion() != "HTTP/1.1";
	bool is_accepted_method		= _location->isMethodAccepted(_method);
	bool is_php_file			= _filepath.find(".php?") != string::npos;
	bool is_301					= _request.getURI() == CASE_301;
	bool is_302					= _request.getURI() == CASE_302;
	bool is_unsupported_type	= _content_types.find(_request.getExtension()) == _content_types.end();
	bool is_existing_file		= isExistingFile(_filepath);

	if (_status == NOT_FOUND)	return NOT_FOUND;
	if (is_correct_HTTP)		return HTTP_VERSION_NOT_SUPPORTED;
	if (!is_accepted_method)	return NOT_IMPLEMENTED;
	if (is_301)					return MOVED_PERMANENTLY;
	if (is_302)					return FOUND;
	if (is_unsupported_type)	return UNSUPPORTED_MEDIA_TYPE;
	if (is_php_file)			return OK;
	if (is_existing_file)		return OK;
	return NOT_FOUND;
}

string	Response::getContent()
{
	bool isCGI = _request.getExtension() == ".php" ||  _request.getExtension() == ".py";

	if (_request.getMethod() == "DELETE")	return deleteFile();
	if (_status == MOVED_PERMANENTLY)		return redirect();
	if (_status == FOUND)					return redirect();
	if (_status == NOT_FOUND)				return fileNotFound();
	if (_status != OK)						return createErrorHTML();
	if (isCGI)								return getCGI();
	return(streamFileDataToString(_filepath));
}

string	Response::deleteFile()
{
	if (remove(_filepath.c_str()) != 0) {
		_status = NOT_FOUND;
		return createErrorHTML();
	}
	return "File " + _filepath + " has been deleted";
}

void	Response::uploadFile()
{
	_request.setUploadSucces(false);

	string input_path;
	map<string, string> env = _request.getEnv();
	if (env.find("file_to_upload") == env.end())
		return;
	else
		input_path = env["file_to_upload"];

	if (!isExistingFile(input_path)) {
		_content = fileNotFound();
		return;
	}

	string file_data = streamFileDataToString(input_path);
	string filename = safe_substr(input_path, input_path.rfind("/"), -1);
	string upload_path = _config.getRoot() + "/" + UPLOAD_FOLDER + "/" + filename;
	writeStringToFile(file_data, upload_path);

	if (isExistingFile(filename)
		&& streamFileDataToString(filename) == file_data)
		_request.setUploadSucces(true);
}

string Response::redirect()
{
	if ((_status == 301 && std::string(COSTUM_301) == "default")
		|| (_status == 302 && std::string(COSTUM_302) == "default"))
		return createErrorHTML();

	if (_status == 301)
		_filepath = _config.getRoot() + COSTUM_301;

	if (_status == 302)
		_filepath = _config.getRoot() + COSTUM_302;

	return streamFileDataToString(_filepath);
}

string Response::fileNotFound()
{
	_status = 404;

	if (std::string(COSTUM_404) == "default")
		return createErrorHTML();

	_filepath = _config.getRoot() + COSTUM_404;
	return streamFileDataToString(_filepath);
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

string Response::setCookie()
{
	string value;
	if (_request.getURI() == "/session_logout.php") {
		value = "deleted";
		return "Set-Cookie: sessionID=" + value + "; expires=Thu, 01 Jan 1970 00:00:00 GMT\n";
	}
	if (_request.getURI() == "/cookies_delete.php") {
		value = "deleted";
		return "Set-Cookie: cookie_value=" + value + "; expires=Thu, 01 Jan 1970 00:00:00 GMT\n";
	}

	map<string, string> env = _request.getEnv();
	if (_request.getURI() == "/session_login.php") {
		if (env.find("username") != env.end())
			value = env["username"];
		return "Set-Cookie: sessionID=" + value + "\n";
	}
	if (_request.getURI() == "/cookies.php") {
		if (env.find("cookie_value") != env.end())
			value = env["cookie_value"];
		return "Set-Cookie: cookie_value=" + value + "\n";
	}

	return "";
}

string Response::getCGI()
{
	class CGI CGI(_request, _location, _filepath, _config.getClientMaxBodySize());
	string cgi = CGI.ExecuteCGI();
	if (cgi.find("<!doctype html>") == string::npos)
		return getCGI();
	if (cgi.find("413 Request Entity Too Large") != string::npos)
		_status = REQUEST_ENTITY_TOO_LARGE;
	if (cgi.find("500 Internal Server Error") != string::npos)
		_status = INTERNAL_SERVER_ERROR;
	return(safe_substr(cgi, cgi.find("<!doctype html>"), -1));
}

list<Location*>::iterator Response::findConfigLocation(string target) {
	if (target.rfind("/") == 0)
		return searchLocations("/");

	list<Location*>::iterator it = _config.locations.begin();
	it = searchLocations(target);
	if (it == _config.locations.end())
		return findConfigLocation(go_one_directory_up(target));
	return it;
}

list<Location*>::iterator Response::searchLocations(string target) {
	list<Location*>::iterator it = _config.locations.begin();
	while (it != _config.locations.end()) {
		if ((*it)->getPath() == target)
			return it;
		++it;
	}
	return it;
}
