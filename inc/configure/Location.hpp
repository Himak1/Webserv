#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>

enum HTTPMethods
{
	GET = 0,
	POST,
	DELETE
};

class Location
{
	public:
		Location();
		Location( Location const & src );
		~Location();

		Location &		operator=( Location const & rhs );

		// void		setRoot();
		// void		setPath();
		// void		setAcceptedMethods();
		// void		setAutoIndex();
	private:
		std::string	_root; // single argument
		std::string	_path;
		bool		_acceptedMethods[4]; // variadic arguments
		bool		_autoIndex; // single argument
		std::string	_defaultFile; // single argument
		std::string	_alias;
		std::string	_cgiExtension;
		std::string	_cgiPath;
};

std::ostream &			operator<<( std::ostream & o, Location const & i );

#endif
