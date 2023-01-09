#include "Request.hpp"

#include <iostream>
#include <fstream>

// CONSTRUCTOR
Request::Request() { _env_list.clear(); }

Request::Request(const Request &src) { *this = src; }

// DESTRUCTOR
Request::~Request() { }

// Overload operator
Request &Request::operator = (const Request &src)
{
	if (this != &src) {
		this->_method = src._method;
		this->_uri = src._uri;
		this->_http_version = src._http_version;
		this->_extension = src._extension;
		this->_status = src._status;
		this->_isCGI = src._isCGI;
		this->_env_list = src._env_list;
	}
	return (*this);
}

// PUBLIC FUNCTIONS
void Request::initRequest(string request)
{
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
	if (strings.size() > 2) _http_version = strings[2].substr(0, 8);

	_headers = request;

	checkStatus();
	checkExtension();

	_isCGI = false;
	if (_extension == ".php" || _extension == ".py") {
		_isCGI = true;
		createEnvList();
	}
}

const string Request::getMethod() const { return _method; }
const string Request::getURI() const { return _uri; }
const string Request::getHTTPVersion() const { return _http_version; }
const string Request::getExtension() const { return _extension; }
int			 Request::getStatus() const { return _status; }
list<string> Request::getEnv() const { return _env_list; };
bool		 Request::isCGI() const { return _isCGI; }

// PRIVATE FUNCTIONS
void Request::checkStatus()
{
	_status = OK;
	if (_http_version != "HTTP/1.1")
		_status = HTTP_VERSION_NOT_SUPPORTED;
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		_status = NOT_IMPLEMENTED;
}

void Request::checkExtension()
{
	const size_t extension_start = _uri.rfind('.');
	if (extension_start == string::npos)
		_extension = ".php";
	else
		_extension = _uri.substr(extension_start, _uri.length());

	if (_extension.find("?"))
		_extension = _extension.substr(0, _extension.find("?"));
}

void Request::createEnvList()
{
	string 	entry;
	string	query;
	size_t	pos;
	string	line;

	_env_list.clear();
	if (_method == "GET") {
		istringstream ss(_uri.substr(_uri.find("?") + 1));
		while (getline(ss, line, '&')) {
			pos = line.find('=');
			entry = line.substr(0, pos) + "=" + line.substr(pos + 1);
			_env_list.push_back(entry);			
	    }
	}
	// TO DO? will not work if input fields are empty. 
	// Currently solved with 'required' tag in html form
	if (_method == "POST") {
		query = _headers;

		istringstream ss(_headers.substr(_headers.find("name=")));
		string previous_line;
		string key;
		int start;
		int end;
		while (getline(ss, line)) {
			if (line.find("name=\"") != string::npos) {
				start = line.find("name=\"") + 6;
				end = line.rfind("\"") - start;
				key = line.substr(start, end);
			}
			if (line.find("------We") != string::npos)
				_env_list.push_back(key + "=" + previous_line);
			previous_line = line;
	    }
	}
}
