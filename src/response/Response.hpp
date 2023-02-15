#ifndef BUILDRESPONSE_HPP
# define BUILDRESPONSE_HPP

# include "../configure/Configuration.hpp"
# include "../request/Request.hpp"
# include "../defines.hpp"

# include <string>
# include <map>

using namespace std;

class Response
{
	public:
		Response(class Request request, class Configuration config);
		~Response();
		string						getMessage();
		string						getFilepath();

	private:
		string						_method;
		string						_filepath;
		int							_status;
		string						_content;
		class Request				_request;
		class Configuration			_config;
		map<int, string>			_status_codes;
		map<string, string>			_content_types;

		void						initStatusCodes();
		void						initContentTypes();
		bool						isExistingFile(string filename);
		int							setStatus();
		string						getContent();
		string 						deleteFile();
		void 						uploadFile();
		string						streamFileDataToString(string input_path);
		void						writeStringToFile(string file_data, string filename);
		string						getCGI();
		string						setCookie();
		string 						redirect();
		string 						fileNotFound();
		string						createErrorHTML();
		string						getFileContent();
		string						createResponse();
};

#endif