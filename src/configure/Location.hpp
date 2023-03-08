#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include "AConfig.hpp"

using namespace std;

class Node;

class Location : public AConfig
{
	public:
		Location( Node* locationNode );
		~Location();

		string	getPath() const;
		string	getAlias() const;
		string	getCgiExtension() const;
		string	getCgiPath() const;
		bool	isMethodAccepted( string& httpMethod ) const;
		bool	autoIndexingOn() const;	
	private:
		void	convertLocation( Node* locationNode );
		void	convertPath( Node* node );
		void	convertCgiPass( Node* node );
		void	convertAcceptedMethods( Node* node );
		void	convertAutoIndex( Node* autoIndex );

		string	_path;
		string	_alias;
		string	_cgiExtension;
		string	_cgiPath;
		string	_acceptedMethods[4];
		bool	_autoIndex;
};

ostream&	operator<<( ostream& o, const Location& location );

class InvalidValueInLocationBlock : public exception
{
	public:
		const char*	what() const throw() {
			return "Invalid value specified in location block";
		}
};

#endif
