#ifndef CGI_HPP
# define CGI_HPP

# include "../server/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>

using namespace std;

class CGI
{
	public:
		CGI(class Request request, class Configuration config);
		~CGI();
		string						ExecuteCGI();
		char** 						getFormEnv() const;

	private:
		const class Request			_request;
		const class Configuration	_config;
		char*						_pathScript;
		char*						_path[2];
		char**						_env;

		char**						createEnv(string query_string);
		void						freeEnv();
};

#endif
