#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include "AConfig.hpp"

class Node;
class Location;

class Configuration : public AConfig
{
	public:
		Configuration( Node* serverNode );
		~Configuration();

		std::string		getIP() const;
		unsigned int	getPort() const;
		unsigned int	getClientMaxBodySize() const;

		std::list<Location>	locations;
	private:
		bool					navigateNode( Node* serverNode );

		std::string				_ipAddress;
		std::string				_host; // server_name
		unsigned int			_port; // listen
		unsigned int			_clientMaxBodySize;
};

#endif
