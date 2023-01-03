#include "Request.hpp"

#include <iostream>
#include <fstream>

// CONSTRUCTOR
Request::Request()
{
	_http_version = "HTTP/1.1";
	_extension = ".html";
}

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
		// cout << "line: " << token << endl;
		stringstream token_ss(token);
		string sub_token;
		while (getline(token_ss, sub_token, ' ')) {
			strings.push_back(sub_token);
		}
	}

	if (strings.size() > 0) _method = strings[0];
	if (strings.size() > 1) _uri = strings[1];
	if (strings.size() > 2) _http_version = strings[2].substr(0, 8);

	checkStatus();
	cout << "_status = " << _status << endl;

	checkExtension();

	// if (_method == "POST" && _uri == "/upload")
		// handleFileUpload(request);
}

const string Request::getMethod() const { return _method; }
const string Request::getURI() const { return _uri; }
const string Request::getHTTPVersion() const { return _http_version; }
const string Request::getExtension() const { return _extension; }
int			 Request::getStatus() const { return _status; }

// PRIVATE FUNCTIONS
void Request::checkStatus()
{
	_status = 200;
	if (_http_version != "HTTP/1.1")
		_status = 400;
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
		_status = 400;
	cout << "_method = " << _method << endl;
}

void Request::checkExtension()
{
	const size_t extension_start = _uri.rfind('.');
	if (extension_start == string::npos)
		_extension = ".html";
	else
		_extension = _uri.substr(extension_start, _uri.length());
}

// TO DO: not working without CGI?
// void Request::handleFileUpload(string request)
// {
//  	// Parse the form data to find the file
// 	istringstream dataStream(request);
// 	string boundary;
// 	string line;

// 	while (getline(dataStream, line)) {
// 		// cout << "line :" << line << endl;

// 		// Look for the boundary line
// 		if (line.substr(0, 2) == "--") {
// 			boundary = line;
// 			break;
// 		}
// 	}

// 	// No boundary line was found, return an error
// 	if (boundary.empty()) {
// 		// TO DO: return error
// 		return;
// 	}

// 	// Read the file data
// 	string fileData;
// 	while (getline(dataStream, line)) {
// 		// Stop reading when we reach the end boundary
// 		if (line == boundary + "--") {
// 			break;
// 		}
// 		fileData += line;
// 	}

// 	// Save the file to a location on the server
// 	ofstream file("/path/to/uploaded/file.txt", ios::binary);
// 	file << fileData;
// 	file.close();

// 	// Send a success response
// 	// TO DO:
// 	cout << "HTTP/1.1 200 OK\r\n\r\n";
// }
