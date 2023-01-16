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
		Request(const Request &src);
		~Request();
		Request &operator = (const Request &src);

		void				initRequest(string request);
		const string		getMethod() const;
		const string		getURI() const;
		const string		getHTTPVersion() const;
		const string		getExtension() const;
		map<string, string>	getEnv() const;
		map<string, string>	getCookies() const;
		bool				isCGI() const;
		bool				getUploadSucces() const;
		void				setUploadSucces(bool result);

	private:
		string				_method;
		string 				_uri;
		string				_http_version;
		string				_extension;
		string				_headers;
		bool				_isCGI;
		map<string, string>	_env;
		map<string, string>	_cookies;
		bool				_is_succesfull_uploaded;

		void				parseHTTPInfoAndHeaders(string request);
		void				parseExtension();
		void				parseEnv();
		void				parseCookies();
		// void 				handleFileUpload(string request);
};

#endif
