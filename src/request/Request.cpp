#include "Request.hpp"

#include <iostream>
#include <fstream>

// CONSTRUCTOR
Request::Request()
{
	_http_version = "HTTP/1.1";
	_validity = false;
}

Request::Request(const Request &src) { *this = src; }

// DESTRUCTOR
Request::~Request() { }

// Overload operator
Request &Request::operator = (const Request &src) {
	if (this != &src) {
		this->_method = src._method;
		this->_uri = src._uri;
		this->_http_version = src._http_version;
		this->_validity = src._validity;
	}
	return (*this);
}

// PUBLIC FUNCTIONS
void Request::initRequest(std::string request)
{
	std::vector<std::string> strings;
	std::stringstream ss(request);
	std::string token;

	while (std::getline(ss, token)) {
		// std::cout << "line: " << token << std::endl;
		std::stringstream token_ss(token);
		std::string sub_token;
		while (std::getline(token_ss, sub_token, ' ')) {
			strings.push_back(sub_token);
		}
	}

	if (strings.size() > 0) _method = strings[0];
	if (strings.size() > 1) _uri = strings[1];
	if (strings.size() > 2) _http_version = strings[2].substr(0, 8);

	checkValidity();

	if (_method == "POST" && _uri == "/upload")
		handleFileUpload(request);
}

bool 			  Request::isValidMethod() const { return _validity; }
const std::string Request::getMethod() const { return _method; }
const std::string Request::getURI() const { return _uri; }
const std::string Request::getHTTPVersion() const { return _http_version; }

// PRIVATE FUNCTIONS
void Request::checkValidity()
{
	_validity = false;
	if (_http_version == "HTTP/1.1"
		&& (_method == "GET" || _method == "POST" || _method == "DELETE"))
		_validity = true;
}

// TO DO: not working without CGI?
void Request::handleFileUpload(std::string request)
{
 	// Parse the form data to find the file
	std::istringstream dataStream(request);
	std::string boundary;
	std::string line;

	while (std::getline(dataStream, line)) {
		// std::cout << "line :" << line << std::endl;

		// Look for the boundary line
		if (line.substr(0, 2) == "--") {
			boundary = line;
			break;
		}
	}

	// No boundary line was found, return an error
	if (boundary.empty()) {
		// TO DO: return error
		return;
	}

	// Read the file data
	std::string fileData;
	while (std::getline(dataStream, line)) {
		// Stop reading when we reach the end boundary
		if (line == boundary + "--") {
			break;
		}
		fileData += line;
	}

	// Save the file to a location on the server
	std::ofstream file("/path/to/uploaded/file.txt", std::ios::binary);
	file << fileData;
	file.close();

	// Send a success response
	// TO DO:
	std::cout << "HTTP/1.1 200 OK\r\n\r\n";
}
