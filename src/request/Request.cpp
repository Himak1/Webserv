#include "Request.hpp"
#include "../utils/strings.hpp"
#include "../utils/fileHandling.hpp"

#include <iostream>
#include <fstream>

// CONSTRUCTORS
Request::Request(Configuration config) : _config(config) { }

Request::Request(Request &src) { *this = src; }

Request &Request::operator = ( Request &src)
{
	if (this != &src) {
		this->_config = src._config;
		this->_location = src._location;
		this->_method = src._method;
		this->_uri = src._uri;
		this->_http_version = src._http_version;
		this->_extension = src._extension;
		this->_headers = src._headers;
		this->_filepath = src._filepath;
		this->_env = src._env;
		this->_is_upload = src._is_upload;
		this->_status = src._status;
	}
	return (*this);
}

// DESTRUCTOR
Request::~Request() { }

// PUBLIC FUNCTIONS
void Request::initRequest(const string &request)
{
	// FILL vars based on server block config_file
	_status = OK;
	_is_upload = false;
	if (request.find("upload?file=") != string::npos)
		_is_upload = true;
	parseHTTPInfoAndHeaders(request);
	setLocation();
	setFilePath();
	_status = setStatus();
	// cout << _status << endl;
	// cout << _filepath << endl;
	// cout << _extension << endl;
	parseEnv();
}

const string 		Request::getMethod() 	  const { return _method; }
const string 		Request::getURI() 		  const { return _uri; }
const string 		Request::getHTTPVersion() const { return _http_version; }
	  string 		Request::getExtension()   const { return _extension; }
const string 		Request::getHeader() 	  const { return _headers; }
map<string, string> Request::getEnv() 	  	  const { return _env; }
bool 		 		Request::isFileUpload()   const { return _is_upload; }
class Location*		Request::getLocation() 	  const { return _location; }
const string 		Request::getFilepath() 	  const { return _filepath; }
int 				Request::getStatus() 	  const { return _status; }

// PRIVATE FUNCTIONS
void Request::parseHTTPInfoAndHeaders(const string& request)
{
	_headers = request;

	vector<string> strings;
	stringstream ss(request);
	string token;

	while (getline(ss, token)) {
		stringstream token_ss(token);
		string sub_token;
		while (getline(token_ss, sub_token, ' ')) {
			strings.push_back(sub_token);
		}
	}
	if (strings.size() > 0) _method = strings[0];
	if (strings.size() > 1) _uri = strings[1];
	if (strings.size() > 2) _http_version = safe_substr(strings[2], 0, 8);
}

void Request::parseEnv()
{
	_env.clear();
	parseCookies();
	parseGet();
	parsePost();

	if (_location->autoIndexingOn())
		_env.insert(pair<string, string> ("directory_listing", "true"));
	else
		_env.insert(pair<string, string> ("directory_listing", "false"));

	_env.insert(pair<string, string> ("upload_directory", _location->getUploadStore()));
}

void Request::parseCookies()
{
	string	cookie_input = safe_substr(_headers, _headers.find("Cookie:") + 7, -1);
	cookie_input = safe_substr(cookie_input, 0, cookie_input.find("\n"));

	string 	line;
	istringstream ss(cookie_input);
	while (getline(ss, line, ';')) {
		unsigned long pos = line.find('=');
		if (pos == string::npos)
			break;
		string key = trim_spaces(safe_substr(line, 0, pos));
		string value = trim_spaces(safe_substr(line, pos + 1, -1));
		_env.insert(pair<string, string> (key, value));
	}
}

void Request::parseGet()
{
	string	line;
	if (_uri.find("?") != string::npos) {
		string value;
		istringstream ss(safe_substr(_uri, _uri.find("?") + 1, -1));
		while (getline(ss, line, '&')) {
			unsigned long pos = line.find('=');
			if (pos == string::npos)
				break;
			string key = trim_spaces(safe_substr(line, 0, pos));
			if (pos + 1 == line.length())
				value = "";
			else
				value = trim_spaces(safe_substr(line, pos + 1, -1));
			_env.insert(pair<string, string> (key, value));
	    }
	}
}

void Request::parsePost() 
{
	string	line;
	if (_method == "POST") {
		string query = _headers;

		istringstream ss(safe_substr(_headers, _headers.find("Content-Disposition:"), -1));
		string previous_line;
		string key;
		int start;
		int end;
		while (getline(ss, line)) {
			if (line.find("name=\"") != string::npos) {
				start = line.find("name=\"") + 6;
				end = line.rfind("\"") - start;
				key = safe_substr(line, start, end);
			}
			if (line.find("------We") != string::npos)
				_env.insert(pair<string, string> (key, previous_line));
			previous_line = line;
	    }
	}
}

void	Request::setLocation()
{
	string target = "www/" + _uri;
	cout << "_config.getRoot()" << _config.getRoot() << endl;
	// string target = _config.getRoot() + _uri;
	list<Location*>::const_iterator it = findConfigLocation(target);
	if (it == _config.locations.end()) {
		_status = INTERNAL_SERVER_ERROR;
	}
	_location = (*it);
}

list<Location*>::const_iterator Request::findConfigLocation(string target) {
	if (target.rfind("/") == 0)
		return searchLocations("/");
	list<Location*>::const_iterator it = searchLocations(target);
	if (it == _config.locations.end())
		return findConfigLocation(go_one_directory_up(target));
	return it;
}

list<Location*>::const_iterator Request::searchLocations(string target) {
	list<Location*>::const_iterator it = _config.locations.begin();
	while (it != _config.locations.end()) {
		// cout << target << endl;
		if ((*it)->getPath() == target) {
			// cout << "match " << target << endl;
			return it;
		}
		++it;
	}
	return it;
}

void	Request::setFilePath()
{
	// _filepath = _config.getRoot() + _uri;
	_filepath = "www/" + _uri;
	_extension = parseExtension(_filepath);

	if ((*_location).getAlias() != "") {
		_filepath = (*_location).getAlias();
		_extension = parseExtension(_filepath);
		return;
	}

	bool is_undefined_extension = _method != "DELETE"
									&& (*_location).getRedirect() == 0
									&& _extension == "";
	// cout << "is_undefined_extension " << is_undefined_extension<< endl;
	if (is_undefined_extension) {	
		if (tryAndSetExtension(".html")) 				return;
		if (tryAndSetExtension(".htm")) 				return;
		if (tryAndSetExtension(".php")) 				return;
		if (tryAndSetExtension(".py")) 					return;
		if (searchIndexFiles((*_location).indexFiles)) 	return;
		if (searchIndexFiles(_config.indexFiles)) 		return;
	}
}

bool	Request::searchIndexFiles(list<string> index_files)
{
	_status = OK;
	list<string>::iterator it = index_files.begin();
	while (it != index_files.end()) {
		// _filepath = _config.getRoot() + _uri + "/" + *it;
		_filepath = "www/" + _uri + "/" + *it;
		if (isExistingFile(_filepath)) {
			_extension = parseExtension(_filepath);
			return true;
		}
		++it;
	}
	if (it == index_files.end())
		_status = NOT_FOUND;
	return false;
}

bool	Request::tryAndSetExtension(string extension)
{
	// _filepath = _config.getRoot() + _uri + extension;
	_filepath = "www/" + _uri + extension;
	if (isExistingFile(_filepath)) {
		_extension = extension;
		return true;
	}
	return false;
}

int		Request::setStatus()
{
	bool is_page_not_found			= _status == NOT_FOUND
									&& _extension != ".png"
									&& _extension != ".jpg"
									&& _extension != ".ico"
									&& _extension != ".css";
	bool is_accepted_method			= (*_location).isMethodAccepted(_method);
	bool is_correct_HTTP			= _http_version == "HTTP/1.1";
	bool is_redirect				= (*_location).getRedirect() != 0;

	// cout << "is_accepted_method " << is_accepted_method<< endl;
	// cout << "is_page_not_found " << is_page_not_found << endl;
	// cout << "is_correct_HTTP " << is_correct_HTTP << endl;
	// cout << "is_redirect " << is_redirect << endl;
	// cout << "_filepath " << _filepath << endl;
	// cout << "isExistingFile(_filepath) " << isExistingFile(_filepath) << endl;

	// bool is_unsupported_type		= _content_types.find(_extension) == _content_types.end();

	if (is_page_not_found)			return NOT_FOUND;
	if (!is_correct_HTTP)			return HTTP_VERSION_NOT_SUPPORTED;
	if (!is_accepted_method)		return METHOD_NOT_ALLOWED;
	if (is_redirect)				return (*_location).getRedirect();
	// if (is_unsupported_type)		return UNSUPPORTED_MEDIA_TYPE;
	if (_extension == ".php")		return OK;
	if (isExistingFile(_filepath))	return OK;
	return NOT_FOUND;		
}