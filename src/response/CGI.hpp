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
		CGI(class Request request, class Location* location, string filepath, int clientMaxBodySize);
		~CGI();
		string						ExecuteCGI();
		char** 						getFormEnv() const;

	private:
		const class Request			_request;
		const class Location*		_location;
		string						_filepath;
		unsigned int				_clientMaxBodySize;
		char*						_buffer;
		char*						_path_to_script;
		char*						_argument;
		char*						_path[4];
		char**						_env;

		void						createPath();
		void						createEnv();
		void						addToEnv(string value, int i);
		void						freeEnv();
};

#endif
