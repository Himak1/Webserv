#ifndef BUILDRESPONSE_HPP
# define BUILDRESPONSE_HPP

# include <string>

class BuildResponse
{
public:
	BuildResponse(std::string uri);
	~BuildResponse();
	std::string         getMessage();

private:
	std::string         _uri;
	std::string			_filename;

	int 				getFileSize(std::string filename);
	std::string 		fileNotFound();
};

#endif
