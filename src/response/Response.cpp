#include "Response.hpp"
#include "CGI.hpp"
#include "../utils/log.hpp"
#include "../utils/strings.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>

#include <list>

// CONSTRUCTOR
Response::Response(class Request request, class Configuration config)
	: _request(request), _config(config)
{
	if (_request.getURI() == "/")
		_filepath = _config.getPathRoot() +  "/index.html";
	else if (_request.getURI().rfind('.') == string::npos)
		_filepath = _config.getPathRoot() + _request.getURI() + ".php";
	else
		_filepath = _config.getPathRoot() + _request.getURI();

	initStatusCodes();
	initContentTypes();
}

// DESTRUCTOR
Response::~Response() { }

// PUBLIC FUNTIONS
string 	Response::getFilepath() { return _filepath; }

string 	Response::getMessage()
{
	_status = setStatus();

	if (safe_substr(_request.getURI(), 0, _request.getURI().find("?")) == "/upload_handler.php")
		uploadFile();

	if (_request.getMethod() == "DELETE")	_content = deleteFile();
	else if (_status == MOVED_PERMANENTLY)	_content = redirect();
	else if (_status == FOUND)				_content = redirect();
	else if (_status == NOT_FOUND)			_content = fileNotFound();
	else if (_status != OK)					_content = createErrorHTML();
	else if (_request.isCGI())				_content = getCGI();
	else									_content = getFileContent();

	return createResponse();
}

// PRIVATE FUNCTIONS
void	Response::initStatusCodes()
{
	_status_codes[200] = "200 OK\n";
	// _status_codes[201] = "201 Created\n";
	_status_codes[202] = "202 Accepted\n";
	_status_codes[204] = "204 No Content\n";
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
	_content_types[".php"] 	= "Content-Type: text/html; charset=utf-8\n";
	_content_types[".js"] 	= "Content-Type: application/javascript\n";
	_content_types[".gif"] 	= "Content-Type: image/gif\n";
}

bool	Response::isExistingFile() {
	basic_ifstream<char> input_stream(_filepath.c_str());
	if (input_stream.is_open()) {
		input_stream.close();
		return true;
	}
	return false;
}

int		Response::setStatus()
{
	bool is_correct_HTTP		= _request.getHTTPVersion() != "HTTP/1.1";
	bool is_incorrect_method	= _request.getMethod() != "GET" 
									&& _request.getMethod() != "POST" 
									&& _request.getMethod() != "DELETE";
	bool is_php_file			= _filepath.find(".php?") != string::npos;
	bool is_301					= _request.getURI() == CASE_301;
	bool is_302					= _request.getURI() == CASE_302;
	bool is_unsupported_type	= _content_types.find(_request.getExtension()) == _content_types.end();

	if (is_correct_HTTP)		return HTTP_VERSION_NOT_SUPPORTED;
	if (is_incorrect_method)	return NOT_IMPLEMENTED;
	if (is_301)					return MOVED_PERMANENTLY;
	if (is_302)					return FOUND;
	if (is_php_file)			return OK;
	if (isExistingFile())		return OK;
	if (is_unsupported_type)	return UNSUPPORTED_MEDIA_TYPE;
	return NOT_FOUND;
}

string Response::deleteFile()
{
	// cout << "delete: " << _filepath << endl;
	if (remove(_filepath.c_str()) != 0) {
		_status = NOT_FOUND;
		return createErrorHTML();
	}
	return "File " + _filepath + " has been deleted";
}

void Response::uploadFile()
{
	_request.setUploadSucces(false);

	string input_path;
	map<string, string> env = _request.getEnv();
	if (env.find("file_to_upload") != env.end())
			input_path = env["file_to_upload"];

	// stream input to file_data
	ifstream input_stream(input_path.c_str());
	ostringstream ss;
	string line;
	while (getline(input_stream, line))
		ss << line << endl;
	string file_data = ss.str();

	// write file_data to output_path
	string upload_path = _config.getPathRoot() + "/" + UPLOAD_FOLDER + "/" + input_path;
	ofstream fout(upload_path);
	fout << file_data << endl;
    fout.close();

	// check if upload was succesful
	basic_ifstream<char> check_upload(upload_path.c_str());
	if (check_upload.is_open()) {
		check_upload.close();
		_request.setUploadSucces(true);
	}
}

string Response::getCGI()
{
	class CGI CGI(_request, _config, _filepath);
	string cgi = CGI.ExecuteCGI();
	return(safe_substr(cgi, cgi.find("<!doctype html>"), -1));
}

string Response::setCookie()
{
	string value;

	if (_request.getURI() == "/session_logout.php") {
		value = "deleted";
		return "Set-Cookie: sessionID=" + value + "; expires=Thu, 01 Jan 1970 00:00:00 GMT\n";
	}

	map<string, string> env = _request.getEnv();
	if (_request.getURI() == "/session_login.php") {
		if (env.find("username") != env.end())
			value = env["username"];
		return "Set-Cookie: sessionID=" + value + "\n";
	}

	return "";
}

string Response::redirect()
{
	if ((_status == 301 && COSTUM_301 == "default")
		|| (_status == 302 && COSTUM_302 == "default"))
		return createErrorHTML();

	if (_status == 301)
		_filepath = _config.getPathRoot() + COSTUM_301;

	if (_status == 302)
		_filepath = _config.getPathRoot() + COSTUM_302;

	return getFileContent();
}

string Response::fileNotFound()
{
	_status = 404;

	if (COSTUM_404 == "default")
		return createErrorHTML();

	_filepath = _config.getPathRoot() + COSTUM_404;
	return getFileContent();
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
		<< setCookie()
		<< "Content-Length: "
		<< _content.size() << "\n\n"
		<< _content;

	// if (_request.getExtension() == ".php")
	// 	cout << "RESPONS: " << ss.str() << endl;
	return ss.str();
}
