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
#include <climits>
#include <string>
#include <list>

// CONSTRUCTOR
Response::Response(const class Request& request, const class Configuration& config)
	: _request(request), _config(config), _location()
{
	_cgi_count = 0;
	_extension = _request.getExtension();
	initStatusCodes();
	initContentTypes();
	setLocation();
	setFilePath();
	_status = setStatus();
	_content = getContent();
}

// DESTRUCTOR
Response::~Response() { }

// PUBLIC FUNTIONS
string 	Response::getFilepath() { return _filepath; }

string 	Response::getMessage()
{
	if (_request.isFileUpload())
		return uploadFile();
	if (_request.getMethod() == "DELETE")
		return deleteFile();

	return  _request.getHTTPVersion() + " "
			+ _status_codes[_status]
			+ _content_types[ _extension]
			+ setCookie()
			+ "Content-Length: "
			+ convertToString(_content.size()) + "\n\n"
			+ _content;
}

// PRIVATE FUNCTIONS
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
	_status_codes[502] = "502 Bad Gateway\n";
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
	_content_types[".py"] 	= "Content-Type: text/html; charset=utf-8\n";
}

int		Response::setStatus()
{
	string method = _request.getMethod();
	bool is_accepted_method			= (*_location).isMethodAccepted(method);

	bool is_too_large_entity = false;
	if ((*_location).getClientMaxBodySize() != UINT_MAX)
		is_too_large_entity		= _request.getPostBodySize() > (*_location).getClientMaxBodySize();
	else if (_config.getClientMaxBodySize() != UINT_MAX)
		is_too_large_entity		= _request.getPostBodySize() > _config.getClientMaxBodySize();

	bool is_page_not_found			= _status == NOT_FOUND
									&& _extension != ".png"
									&& _extension != ".jpg"
									&& _extension != ".ico"
									&& _extension != ".css";
	bool is_correct_HTTP			= _request.getHTTPVersion() != "HTTP/1.1";
	bool is_redirect				= (*_location).getRedirect() != 0;
	bool is_unsupported_type		= _content_types.find(_extension) == _content_types.end();

	if (is_page_not_found)			return NOT_FOUND;
	if (is_too_large_entity)		return REQUEST_ENTITY_TOO_LARGE;
	if (is_correct_HTTP)			return HTTP_VERSION_NOT_SUPPORTED;
	if (!is_accepted_method)		return METHOD_NOT_ALLOWED;
	if (is_redirect)				return (*_location).getRedirect();
	if (_extension == ".php")		return OK;
	if (!isExistingFile(_filepath))	return NOT_FOUND;
	if (is_unsupported_type)		return UNSUPPORTED_MEDIA_TYPE;
	return OK;
}

string	Response::getContent()
{
	bool isCGI = _extension == ".php" || _extension == ".py";

	if (_status != OK)	return returnErrorPage();
	if (isCGI)			return getCGI();
	return(streamFileDataToString(_filepath));
}

string	Response::deleteFile()
{
	string method = _request.getMethod();
	if (!(*_location).isMethodAccepted(method))
		return "405 Method Not Allowed\n";

	if (remove(_filepath.c_str()) != 0)
		return "HTTP/1.1 400 Bad Request\n";
	return "HTTP/1.1 200 OK\n";
}

string	Response::uploadFile()
{
	string method = _request.getMethod();
	if (!(*_location).isMethodAccepted(method))
		return "HTTP/1.1 405 Method Not Allowed\n";

	map<string, string> env = _request.getEnv();
	if (env.find("file") == env.end())
		return "HTTP/1.1 400 Bad Request\n";

	string input_path = env["file"];
	if (!isExistingFile(input_path))
		return "HTTP/1.1 400 Bad Request\n";

	string file_data = streamFileDataToString(input_path);
	string filename = safe_substr(input_path, input_path.rfind("/"), -1);
	string upload_path = setUploadPath(filename);
	writeStringToFile(file_data, upload_path);
	if (isExistingFile(upload_path))
		return "HTTP/1.1 201 Created\n";
	return "HTTP/1.1 500 Internal Server Error\n";
}

string	Response::setUploadPath(string filename)
{
	string upload_store;
	if ((*_location).getUploadStore() != "")
 		upload_store = (*_location).getUploadStore();
	else if (_config.getUploadStore() != "")
 		upload_store = _config.getUploadStore();
	else
		upload_store = "uploads";
	
	if (upload_store == "/")
		upload_store = "uploads";

	if (upload_store != _config.getRoot())
		mkdir(upload_store.c_str(), 0777);

	return upload_store + filename;
}

string Response::returnErrorPage()
{
	bool costum_error_page_is_defined = _config.getErrorPage(_status) != ""
										|| (*_location).getErrorPage(_status) != "";

	if ((*_location).getRedirect() != 0 || costum_error_page_is_defined) {
		if ((*_location).getErrorPage(_status) != "")
			_filepath = _config.getRoot() + "/" + (*_location).getErrorPage(_status);
		else if (_config.getErrorPage(_status) != "")
			_filepath = _config.getRoot() + "/" + _config.getErrorPage(_status);
		
		if (isExistingFile(_filepath))
			return streamFileDataToString(_filepath);
	}
	return createErrorHTML();
}

string Response::createErrorHTML()
{
	string meta  = "";
	string url = (*_location).getRedirectURI();
	if (_status == MOVED_PERMANENTLY || _status == FOUND)
		meta = "<meta charset=\"utf-8\"/><meta http-equiv=\"refresh\" content=\"3; url=" + url + "\"/>";
	return "<!DOCTYPE html><html lang=\"en\"><head><title>"
			+ _status_codes[_status] + "</title>" + meta + "</head><body><center><h1>"
			+ _status_codes[_status] + "</h1></center></body></html>";
}

string Response::setCookie()
{
	string value;
	if (_request.getURI() == "/session_logout.php")
		return "Set-Cookie: sessionID=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT\n";
	if (_request.getURI() == "/cookies_delete.php")
		return "Set-Cookie: cookie_value=deleted; expires=Thu, 01 Jan 1970 00:00:00 GMT\n";

	map<string, string> env = _request.getEnv();
	if (_request.getURI() == "/session_login.php" && env.find("username") != env.end())
		return "Set-Cookie: sessionID=" + env["username"] + "\n";
	if (_request.getURI() == "/cookies.php" && env.find("cookie_value") != env.end())
		return "Set-Cookie: cookie_value=" + env["cookie_value"] + "\n";

	return "";
}

string Response::getCGI()
{
	class CGI CGI(_request, _location, _filepath);
	string cgi = CGI.ExecuteCGI();
	if (cgi.find("<!doctype html>") == string::npos && _cgi_count++ < 10)
		return getCGI();
	if (cgi.find("413 Request Entity Too Large") != string::npos)
		_status = REQUEST_ENTITY_TOO_LARGE;
	if (cgi.find("502 Bad Gateway") != string::npos)
		_status = BAD_GATEWAY;
	if (cgi.find("500 Internal Server Error") != string::npos)
		_status = INTERNAL_SERVER_ERROR;
	
	if (_status != OK)
		return returnErrorPage();
	return(safe_substr(cgi, cgi.find("<!doctype html>"), -1));
}
