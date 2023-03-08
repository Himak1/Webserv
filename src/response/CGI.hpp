#ifndef CGI_HPP
# define CGI_HPP

# include "../configure/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>

using namespace std;

class CGI
{
	public:
		CGI(const class Request& request, class Location* location, string filepath);
		~CGI();
		string					ExecuteCGI();
		char** 					getFormEnv() const;

	private:
		const class Request		_request;
		const class Location*	_location;
		string					_filepath;
		char*					_buffer;
		char*					_path[4];
		char**					_env;
		string					_code;
		bool					_allocation_has_failed;

		string					pipeAndFork(int output);
		void					removeSleep();
		bool					hasInfiniteLoop(string condition);
		void					createPath();
		void					createEnv();
		void					addToEnv(string value, int i);
		void					freeEnv();
};

#endif
