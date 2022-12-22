#ifndef CGI_HPP
# define CGI_HPP

# include "Configuration.hpp"
# include "HTTPRequest.hpp"

# include <string>

class CGI
{
	public:
		CGI(class HTTPRequest request, class Configuration config);
		~CGI();
		std::string         ExecuteCGI();

	private:
		class HTTPRequest	_request;
		class Configuration	_config;
		char*				_pathSscript;
		char*				_path[2];
};

#endif
