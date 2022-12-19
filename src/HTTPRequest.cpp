#include <HTTPRequest.hpp>

// CONSTRUCTOR
HTTPRequest::HTTPRequest()
{
	_http_version = "HTTP/1.1";
}

// DESTRUCTOR
HTTPRequest::~HTTPRequest()
{
}

// PUBLIC FUNCTIONS
void HTTPRequest::init(std::string request)
{
	std::vector<std::string> strings;
	std::stringstream ss(request);
	std::string token;

	while (std::getline(ss, token))
	{
		std::stringstream token_ss(token);
		std::string sub_token;
		while (std::getline(token_ss, sub_token, ' '))
		{
			strings.push_back(sub_token);
		}
	}

	if (strings.size() > 0) _method = strings[0];
	if (strings.size() > 1) _uri = strings[1];
	if (strings.size() > 2) _http_version = strings[2];
}

const std::string HTTPRequest::getMethod() const { return _method; }
const std::string HTTPRequest::getURI() const { return _uri; }
const std::string HTTPRequest::getHTTPVersion() const { return _http_version; }
