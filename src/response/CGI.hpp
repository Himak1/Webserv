#ifndef CGI_HPP
# define CGI_HPP

# include "../server/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>

using namespace std;

class CGI
{
	public:
		CGI(class Request request, class Configuration config, string filepath);
		~CGI();
		string						ExecuteCGI();
		char** 						getFormEnv() const;

	private:
		const class Request			_request;
		const class Configuration	_config;
		string						_filepath;
		char*						_path_to_cgi;
		char*						_path_to_script;
		char*						_argument;
		char*						_path[4];
		char**						_env;

		char**						createEnv();
		void						freeEnv();
};

#endif
