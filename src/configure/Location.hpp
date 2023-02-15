#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>
# include "AConfig.hpp"

class Node;

class Location : public AConfig
{
	public:
		Location( Node* locationNode );
		Location( const Location& src );
		~Location();

		Location&	operator=( const Location& src );

		std::string	getPath() const;
		std::string	getAlias() const;
		std::string	getCgiExtension() const;
		std::string	getCgiPath() const;
		bool		isMethodAccepted( std::string& httpMethod ) const;
		bool		autoIndexingOn() const;	
	private:
		void	convertLocation( Node* locationNode );
		void	convertPath( Node* node );
		void	convertCgiPass( Node* node );
		void	convertAcceptedMethods( Node* node );
		void	convertAutoIndex( Node* autoIndex );

		std::string	_path;
		std::string	_alias;
		std::string	_cgiExtension;
		std::string	_cgiPath;
		std::string	_acceptedMethods[4];
		bool		_autoIndex;
};

std::ostream&	operator<<( std::ostream& o, const Location& location );

#endif
