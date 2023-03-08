#include "Response.hpp"
#include "../utils/fileHandling.hpp"

void	Response::setFilePath()
{
	if ((*_location).getAlias() != "") {
		_filepath = _config.getRoot() + (*_location).getAlias();
		_extension = parseExtension(_filepath);
		return;
	}

	bool is_undefined_extension = _request.getMethod() != "DELETE"
									&& (*_location).getRedirect() == 0
									&& _extension == "";

	if (is_undefined_extension) {	
		if (tryAndSetExtension(".html")) 				return;
		if (tryAndSetExtension(".htm")) 				return;
		if (tryAndSetExtension(".php")) 				return;
		if (tryAndSetExtension(".py")) 					return;
		if (searchIndexFiles((*_location).indexFiles)) 	return;
		if (searchIndexFiles(_config.indexFiles)) 		return;
	}
	_filepath = _config.getRoot() + _request.getURI();
}

bool	Response::tryAndSetExtension(string extension)
{
	_filepath = _config.getRoot() + _request.getURI() + extension;
	if (isExistingFile(_filepath)) {
		_extension = extension;
		return true;
	}
	return false;
}

bool	Response::searchIndexFiles(list<string> index_files)
{
	_status = OK;
	list<string>::iterator it = index_files.begin();
	while (it != index_files.end()) {
		_filepath = _config.getRoot() + _request.getURI() + "/" + *it;
		if (isExistingFile(_filepath)) {
			_extension = parseExtension(_filepath);
			return true;
		}
		++it;
	}
	if (it == index_files.end())
		_status = NOT_FOUND;
	return false;
}
