#ifndef BUILDRESPONSE_HPP
# define BUILDRESPONSE_HPP

# include "../server/Configuration.hpp"
# include "../request/Request.hpp"

# include <string>
# include <map>

class Response
{
	public:
		Response(class Request request, class Configuration config);
		~Response();
		std::string         getMessage();
		std::string			getFilename();

	private:
		std::string			_filename;
		std::string			_status;
		std::string			_content;
		class Request		_request;
		class Configuration	_config;
		std::map<int, std::string>_status_codes;
		std::map<std::string, std::string>_content_types;

		void				initStatusCodes();
		void				initContentTypes();
		std::string			readStream(std::string filename);
		std::string 		fileNotFound();
		std::string			createResponse();
};

#endif
