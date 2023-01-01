#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <vector>
# include <sstream>

class Request
{
	public:
		Request();
		Request(const Request &src);
		~Request();
		Request &operator = (const Request &src);

		void						initRequest(std::string request);
		bool						isValidMethod() const;
		const std::string			getMethod() const;
		const std::string			getURI() const;
		const std::string			getHTTPVersion() const;

	private:
		std::string					_method;
		std::string 				_uri;
		std::string					_http_version;
		// std::vector<std::string>	_headers;
		bool						_validity;

		void						checkValidity();
		void 						handleFileUpload(std::string request);
};

#endif
