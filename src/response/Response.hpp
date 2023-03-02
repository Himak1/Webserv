#ifndef BUILDRESPONSE_HPP
# define BUILDRESPONSE_HPP

# include "../configure/Configuration.hpp"
# include "../configure/Location.hpp"
# include "../request/Request.hpp"
# include "../defines.hpp"

# include <string>
# include <map>

using namespace std;

class Response
{
	public:
		Response(const class Request& request, const class Configuration& config);
		~Response();
		string							getMessage();
		string							getFilepath();

	private:
		string							_filepath;
		string							_extension;
		int								_status;
		string							_content;
		const class Request&			_request;
		const class Configuration&		_config;
		class Location*					_location;
		map<int, string>				_status_codes;
		map<string, string>				_content_types;
		int								_cgi_count;
	
		void							setFilePath();
		bool							tryAndSetExtension(string extension);
		bool							searchIndexFiles(list<string> index_files);
		void							setLocation();
		void							initStatusCodes();
		void							initContentTypes();
		int								setStatus();
		string							getContent();
		string 							deleteFile();
		string 							uploadFile();
		string							setUploadPath(string filename);
		string							getCGI();
		string							setCookie();
		string 							returnErrorPage();
		string							createErrorHTML();
		list<Location*>::const_iterator	findConfigLocation(string target);
		list<Location*>::const_iterator	searchLocations(string target);
};

#endif
