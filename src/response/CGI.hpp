#ifndef CGI_HPP
# define CGI_HPP

# include "../configure/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>

using namespace std;

class CGI
{
	public:
		// CGI(class Request request, class Configuration config, string filepath);
		CGI(class Request request, class Location& location, string filepath);
		~CGI();
		string						ExecuteCGI();
		char** 						getFormEnv() const;

	private:
		const class Request			_request;
		const class Location&		_location;
		string						_filepath;
		char*						_path_to_script;
		char*						_argument;
		char*						_path[4];
		char**						_env;

		char**						createEnv();
		void						freeEnv();
};

#endif
