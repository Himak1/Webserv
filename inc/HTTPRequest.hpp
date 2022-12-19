#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>
# include <vector>
# include <sstream>

class HTTPRequest
{
	public:
		HTTPRequest();
		~HTTPRequest();

		void				init(std::string request);
		const std::string	getMethod() const;
		const std::string	getURI() const;
		const std::string	getHTTPVersion() const;

	private:
		std::string			_method;
		std::string 		_uri;
		std::string			_http_version;
};

#endif
