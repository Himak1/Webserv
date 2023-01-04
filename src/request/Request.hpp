#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../defines.hpp"

# include <string>
# include <vector>
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
		int					getStatus() const;

	private:
		string				_method;
		string 				_uri;
		string				_http_version;
		string				_extension;
		int					_status;

		void				checkStatus();
		void				checkExtension();
		// void 				handleFileUpload(string request);
};

#endif
