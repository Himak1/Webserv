#include "HTTPRequest.hpp"

# include <fstream>

// CONSTRUCTOR
HTTPRequest::HTTPRequest()
{
	_http_version = "HTTP/1.1";
	_validity = false;
}

HTTPRequest::HTTPRequest(const HTTPRequest &src) { *this = src; }

// DESTRUCTOR
HTTPRequest::~HTTPRequest() { }

// Overload operator
HTTPRequest &HTTPRequest::operator = (const HTTPRequest &src) {
	if (this != &src) {
		this->_method = src._method;
		this->_uri = src._uri;
		this->_http_version = src._http_version;
		this->_validity = src._validity;
	}
	return (*this);
}

// PUBLIC FUNCTIONS
void HTTPRequest::initHTTPRequest(std::string request)
{
	std::vector<std::string> strings;
	std::stringstream ss(request);
	std::string token;

	while (std::getline(ss, token)) {
		std::stringstream token_ss(token);
		std::string sub_token;
		while (std::getline(token_ss, sub_token, ' ')) {
			strings.push_back(sub_token);
		}
	}

	if (strings.size() > 0) _method = strings[0];
	if (strings.size() > 1) _uri = strings[1];
	if (strings.size() > 2) _http_version = strings[2];

	checkValidity();

	// if (_method == "POST" && _uri == "/upload")
		// handleFileUpload(request);
}

bool 			  HTTPRequest::isValidMethod() const { return _validity; }
const std::string HTTPRequest::getMethod() const { return _method; }
const std::string HTTPRequest::getURI() const { return _uri; }
const std::string HTTPRequest::getHTTPVersion() const { return _http_version; }

// PRIVATE FUNCTIONS
void HTTPRequest::checkValidity()
{
	if (_method == "GET" || _method == "POST" || _method == "DELETE")
		_validity = true;
}
#include <iostream>
// void HTTPRequest::handleFileUpload(std::string request)
// {
//  	// Parse the form data to find the file
// 	std::istringstream dataStream(request);
// 	std::string boundary;
// 	std::string line;
// 	while (std::getline(dataStream, line))
// 	{
// 			std::cout << "line :" << line << std::endl;
// 		// Look for the boundary line
// 		if (line.substr(0, 2) == "--")
// 		{
// 			boundary = line;
// 			break;
// 		}
// 	}

// 	if (boundary.empty())
// 	{
// 		// No boundary line was found, return an error
// 		std::cout << "No boundary line was found\n" << std::endl;
// 		return;
// 	}

// 	// Read the file data
// 	std::string fileData;
// 	while (std::getline(dataStream, line))
// 	{
// 		// Stop reading when we reach the end boundary
// 		if (line == boundary + "--")
// 		{
// 			std::cout << "BOUNDARY " << line << std::endl;
// 			break;
// 		}

// 		fileData += line;
// 	}

// 	// Save the file to a location on the server
// 	std::ofstream file("/path/to/uploaded/file.txt", std::ios::binary);
// 	file << fileData;
// 	file.close();

// 	// Send a success response
// 	std::cout << "HTTP/1.1 200 OK\r\n\r\n";
// }

// void HTTPRequest::handleFileUpload(std::string request)
// {
//     // Parse the request to get the content type and boundary string
//     std::string contentType;
//     std::string boundary;
//     std::string line;
// 	std::stringstream ss(request);
//     while (std::getline(ss, line)) {
// 		std::cout << line << std::endl;
//         if (line.find("Content-Type: ") == 0) {
//             contentType = line.substr(15);
// 			std::cout << "CONTENTTYPE = " << contentType << std::endl;
//         }
//         else if (line.find("Content-Disposition: ") == 0) {
//             std::string disposition = line.substr(21);
//             std::size_t pos = disposition.find("boundary=");
//             if (pos != std::string::npos) {
//                 boundary = disposition.substr(pos + 9);
// 				std::cout << "BOUNDARY = " << boundary << std::endl;
//                 // Remove leading and trailing quotes from the boundary string
//                 if (boundary[0] == '"') {
//                     boundary = boundary.substr(1, boundary.size() - 2);
// 					std::cout << "BOUNDARY = " << boundary << std::endl;
//                 }
//             }
//         }
//         else if (line == "\r") {
//             // End of the headers
//             break;
//         }
//     }

//     // Check that we found the content type and boundary string
//     if (contentType.empty() || boundary.empty()) {
//         // std::cerr << "Error: Invalid request format" << std::endl;
//         return;
//     }

//     // Read the contents of the file being uploaded
//     std::stringstream fileStream;
//     bool foundFile = false;
//     while (std::getline(ss, line)) {
//         if (line == "--" + boundary) {
//             // Start of a new part
//             foundFile = false;
//         }
//         else if (line == "--" + boundary + "--") {
//             // End of the request
//             break;
//         }
//         else if (line.find("Content-Disposition: ") == 0) {
//             std::string disposition = line.substr(21);
//             if (disposition.find("form-data; name=\"uploaded_file\"; filename=") == 0) {
// 				std::cout << "FILE " << line << std::endl;
//                 // This is the file we're looking for
//                 foundFile = true;
//             }
//         }
//         else if (line == "\r") {
//             // End of the headers for this part
//         }
//         else if (foundFile) {
//             // Append the line to the file stream
//             fileStream << line << std::endl;
// 			std::cout << "FILE " << line << std::endl;
//         }
//     }

//     // Save the contents of the file stream to a file
//     std::ofstream outputFile("/path/to/save/uploaded_file", std::ios::binary);
//     outputFile << fileStream.rdbuf();
//     outputFile.close();
// }