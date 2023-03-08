#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include <vector>
# include "AConfig.hpp"

using namespace std;

class Node;
class Location;

class Configuration : public AConfig
{
	public:
		Configuration( Node* serverNode );
		~Configuration();

		string			getHost() const;
		unsigned int	getPort() const;

		list<Location*>	locations;
	private:
		void			navigateNode( Node* serverNode );
		void			convertPort( Node* listen );

		string			_host; // server_name
		unsigned int	_port; // listen
};

ostream&	operator<<( ostream& o, const Configuration& config );

#endif
