#include "Request.hpp"
#include "../utils/strings.hpp"

#include <iostream>
#include <fstream>

// CONSTRUCTOR
Request::Request() { }

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
		this->_headers = src._headers;
		this->_env = src._env;
		this->_is_succesfull_uploaded = src._is_succesfull_uploaded;
	}
	return (*this);
}

// PUBLIC FUNCTIONS
void Request::initRequest(string request)
{
	// cout << request << endl;
	// cout << request.size() << endl;
	parseHTTPInfoAndHeaders(request);
	// if (_headers.size() >= 12)
		// return;
	parseEnv();
	parseExtension();
}

const string Request::getMethod() const { return _method; }
const string Request::getURI() const { return _uri; }
const string Request::getHTTPVersion() const { return _http_version; }
const string Request::getExtension() const { return _extension; }
const string Request::getHeader() const { return _headers; }
map<string, string> Request::getEnv() const { return _env; };
bool 		 Request::getUploadSucces() const { return _is_succesfull_uploaded; }
void 		 Request::setUploadSucces(bool result) { _is_succesfull_uploaded = result; }

// PRIVATE FUNCTIONS
void Request::parseHTTPInfoAndHeaders(string request)
{
	_headers = request;

	// if (_headers.size() >= 12)
		// return;
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

void Request::parseExtension()
{
	if (_uri.find('?') != string::npos)
		_uri = safe_substr(_uri, 0, _uri.find('?'));

	size_t extension_start = _uri.rfind('.');
	if (extension_start != string::npos)
		_extension = safe_substr(_uri, extension_start, _uri.length());
	else
		_extension = ".php";
	
}

void Request::parseEnv()
{
	_env.clear();
	parseCookies();
	parseGet();
	parsePost();
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
