#include "Response.hpp"
#include "../utils/strings.hpp"

void	Response::setLocation()
{
	string target = _config.getRoot() + _request.getURI();
	list<Location*>::const_iterator it = findConfigLocation(target);
	if (it == _config.locations.end()) {
		_status = INTERNAL_SERVER_ERROR;
	}
	_location = (*it);
}

list<Location*>::const_iterator Response::findConfigLocation(string target) {
	if (target.rfind("/") == 0)
		return searchLocations("/");
	list<Location*>::const_iterator it = searchLocations(target);
	if (it == _config.locations.end())
		return findConfigLocation(go_one_directory_up(target));
	return it;
}

list<Location*>::const_iterator Response::searchLocations(string target) {
	list<Location*>::const_iterator it = _config.locations.begin();
	while (it != _config.locations.end()) {
		if ((*it)->getPath() == target)
			return it;
		++it;
	}
	return it;
}
