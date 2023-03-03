#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include <vector>
# include "AConfig.hpp"

class Node;
class Location;

class Configuration : public AConfig
{
	public:
		// Configuration();
		Configuration( Node* serverNode );
		// Configuration(const Configuration& src);
		// Configuration &operator = (const Configuration& src);
		~Configuration();

		std::string		getHost() const;
		unsigned int	getPort() const;

		std::list<Location*>	locations;
	private:
		void			navigateNode( Node* serverNode );
		void			convertPort( Node* listen );

		std::string				_host; // server_name
		unsigned int			_port; // listen
};

std::ostream&	operator<<( std::ostream& o, const Configuration& config );

#endif
