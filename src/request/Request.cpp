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
		this->_isCGI = src._isCGI;
		this->_env_list = src._env_list;
		this->_cookies = src._cookies;
	}
	return (*this);
}

// PUBLIC FUNCTIONS
void Request::initRequest(string request)
{
	parseHTTPInfoAndHeaders(request);
	parseExtension();

	_isCGI = false;
	if (_extension == ".php" || _extension == ".py")
		_isCGI = true;

	parseEnv();
	parseCookies();
}

const string Request::getMethod() const { return _method; }
const string Request::getURI() const { return _uri; }
const string Request::getHTTPVersion() const { return _http_version; }
const string Request::getExtension() const { return _extension; }
list<string> Request::getEnv() const { return _env_list; };
bool		 Request::isCGI() const { return _isCGI; }
bool 		 Request::getUploadSucces() const { return _is_succesfull_uploaded; }
void 		 Request::setUploadSucces(bool result) { _is_succesfull_uploaded = result; }

// PRIVATE FUNCTIONS
void Request::parseHTTPInfoAndHeaders(string request)
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
	if (strings.size() > 2) _http_version = safe_substr(strings[2], 0, 8);

	_headers = request;
}

void Request::parseExtension()
{
	const size_t extension_start = _uri.rfind('.');
	if (extension_start == string::npos)
		_extension = ".php";
	else
		_extension = safe_substr(_uri, extension_start, _uri.length());

	// om ervoor te zorgen dat upload_handler.php?file_to_upload=filename goed wordt geladen
	if (_uri.find(".php?") != string::npos)
		_extension = ".php";

}

void Request::parseEnv()
{
	string	line;

	_env_list.clear();

	if (_method == "GET") {
		istringstream ss(safe_substr(_uri, _uri.find("?") + 1, -1));
		while (getline(ss, line, '&')) {
			int pos = line.find('=');
			if (pos == string::npos)
				break;
			string entry = safe_substr(line, 0, pos) + "=" + safe_substr(line, pos + 1, -1);
			_env_list.push_back(entry);			
	    }
	}
	// TO DO? will not work if input fields are empty. 
	// Currently solved with 'required' tag in html form
	else if (_method == "POST") {
		string query = _headers;

		istringstream ss(safe_substr(_headers, _headers.find("name="), -1));
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
				_env_list.push_back(key + "=" + previous_line);
			previous_line = line;
	    }
	}
}

void	Request::parseCookies()
{
	string 	line;
	string	cookies = safe_substr(_headers, _headers.find("Cookie:") + 7, -1);
	cookies = safe_substr(cookies, 0, cookies.find("\n"));
	_cookies.clear();

	istringstream ss(cookies);
	while (getline(ss, line, ';')) {
		int pos = line.find('=');
		if (pos == string::npos)
			break;
		string key = trim_spaces(safe_substr(line, 0, pos));
		string value = trim_spaces(safe_substr(line, pos + 1, -1));
		_cookies.insert({key, value});
	}

	// for (map<string, string>::iterator it = _cookies.begin(); it != _cookies.end(); ++it) {
	// 	cout << it->first << ": " << it->second << endl;
	// }
}

// void handleLogin(std::string& user, std::string& password) {
//     // check if user and password are valid
//     // if (!isValid(user, password)) {
//         // return error message to client
//         // return;
//     // }

//     // create a new session for the user
//     std::string sessionID = generateSessionID();
//     Session session;
//     session.user = user;
//     session.expiration = time() + SESSION_TIMEOUT;
//     sessions[sessionID] = session;

//     // create a new "Set-Cookie" header
//     std::string cookie = "session=" + sessionID + "; expires=" + session.expiration;

//     // send "Set-Cookie" header to client
//     std::string headers = "Set-Cookie: " + cookie;
//     sendResponse(headers);
// }