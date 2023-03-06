#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../defines.hpp"

# include <string>
# include <map>
# include <vector>
# include <list>
# include <sstream>

using namespace std;

class Request
{
	public:
		Request();
		~Request();
		Request(const Request& src);
		Request &operator = (const Request& src);

		void				initRequest(const string& request);
		const string		getMethod() const;
		const string		getURI() const;
		const string		getHTTPVersion() const;
		string				getExtension() const;
		const string		getHeader() const;
		map<string, string>	getEnv() const;
		map<string, string>	getCookies() const;
		unsigned int		getPostBodySize() const;
		bool				isCGI() const;
		bool				isFileUpload() const;
 
	private:
		string				_method;
		string 				_uri;
		string				_http_version;
		string				_extension;
		string				_headers;
		map<string, string>	_env;
		bool				_is_upload;
		int					_post_body_size;

		void				parseHTTPInfoAndHeaders(const string& request);
		void				parseEnv();
		void				parseGet();
		void				parsePost();
		void				parseCookies();
};

#endif
