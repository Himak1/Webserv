#ifndef CGI_HPP
# define CGI_HPP

# include "../server/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>

class CGI
{
	public:
		CGI(class Request request, class Configuration config);
		~CGI();
		std::string         ExecuteCGI();

	private:
		class Request		_request;
		class Configuration	_config;
		char*				_pathSscript;
		char*				_path[2];
};

#endif
