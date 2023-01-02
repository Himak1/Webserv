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
		std::string			getFilepath();

	private:
		std::string			_filepath;
		int					_status;
		std::string			_content;
		const class Request		_request;
		const class Configuration	_config;
		std::map<int, std::string>_status_codes;
		std::map<std::string, std::string>_content_types;

		void				initStatusCodes();
		void				initContentTypes();
		int					setStatus();
		std::string 		redirect();
		std::string 		fileNotFound();
		std::string			createErrorHTML();
		std::string			getFileContent();
		std::string			createResponse();
};

#endif
