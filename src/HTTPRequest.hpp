#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <vector>
# include <sstream>

class HTTPRequest
{
	public:
		HTTPRequest();
		HTTPRequest(const HTTPRequest &src);
		~HTTPRequest();
		HTTPRequest &operator = (const HTTPRequest &src);

		void						initHTTPRequest(std::string request);
		bool						isValidMethod() const;
		const std::string			getMethod() const;
		const std::string			getURI() const;
		const std::string			getHTTPVersion() const;

	private:
		bool						_validity;
		std::string					_method;
		std::string 				_uri;
		std::string					_http_version;
		// std::vector<std::string>	_headers;

		void						checkValidity();
		void 						handleFileUpload(std::string request);
};

#endif
