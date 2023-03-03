#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "../defines.hpp"
# include "../configure/Configuration.hpp"
# include "../configure/Location.hpp"

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
		~Request();
		Request(const Request& src);
		Request &operator = (const Request& src);

		void							initRequest(const string& request, const Configuration& config);
		const string					getMethod() const;
		const string					getURI() const;
		const string					getHTTPVersion() const;
		string							getExtension() const;
		const string					getHeader() const;
		map<string, string>				getEnv() const;
		map<string, string>				getCookies() const;
		bool							isCGI() const;
		bool							isFileUpload() const;
		class Location*					getLocation() const;
		const string					getFilepath() const;
		int								getStatus() const;
 
	private:
		class Configuration&			_config;
		class Location*					_location;
		string							_method;
		string 							_uri;
		string							_http_version;
		string							_extension;
		string							_headers;
		string							_filepath;
		map<string, string>				_env;
		bool							_is_upload;
		int								_status;

		void							parseHTTPInfoAndHeaders(const string& request);
		void							parseEnv();
		void							parseGet();
		void							parsePost();
		void							parseCookies();
		void							setLocation();
		list<Location*>::const_iterator	findConfigLocation(string target);
		list<Location*>::const_iterator	searchLocations(string target);
		void							setFilePath();
		bool							searchIndexFiles(list<string> index_files);
		bool							tryAndSetExtension(string extension);
		int								setStatus();
};

#endif
