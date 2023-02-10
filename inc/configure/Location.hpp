#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <string>

class Location : public AConfig
{
	public:
		Location( Node* locationNode );
		Location( Location const& src );
		~Location();

		Location&	operator=( Location const& src );

		std::string	getPath() const;
		std::string	getAlias() const;
		std::string	getCgiExtension() const;
		std::string	getCgiPath() const;
		bool		isMethodAccepted( std::string& httpMethod ) const;
		bool		autoIndexingOn() const;	
	private:
		void	convertLocation( Node* locationNode );

		std::string	_path;
		std::string	_alias;
		std::string	_cgiExtension;
		std::string	_cgiPath;
		std::string	_acceptedMethods[4];
		bool		_autoIndex;
};

#endif
