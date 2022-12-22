#ifndef BUILDRESPONSE_HPP
# define BUILDRESPONSE_HPP

# include <Configuration.hpp>
# include <HTTPRequest.hpp>

# include <string>

class BuildResponse
{
	public:
		BuildResponse(class HTTPRequest request, class Configuration config);
		~BuildResponse();
		std::string         getMessage(std::string status);

	private:
		std::string			_filename;
		class HTTPRequest	_request;
		class Configuration	_config;

		std::string 		fileNotFound();
		std::string 		streamStatus(std::string status) const;
};

#endif
