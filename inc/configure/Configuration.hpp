
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include <utility>
# include "Location.hpp"

/*
class Configuration
{
	public:
		Configuration();
		Configuration(const Configuration &src);
		~Configuration();
		Configuration &operator = (const Configuration &src);

		void       			parseConfiguration(std::string filename);
		std::string			getPathWebsite() const;
		std::string			getIP() const;
		unsigned int		getPort() const;
		std::string			get404() const;

		std::list<Location>	_locations;
	private:
		std::string			_rootPath; // dependency
		std::string			_ipAddress; // dependency
		std::string			_host;
		unsigned int		_port; // dependency
		std::string			_pathWebsite; // dependency
		std::string			_errorPage404; // dependency
		std::list< std::pair<int, std::string> >	_errorPages;
};
*/

/* class Configuration */
/* { */
/* 	public: */
/* 		Configuration(); */
/* 		~Configuration(); */

/* 		void       			parseConfiguration(std::string filename); */
/* 		std::string			getPathWebsite() const; */
/* 		std::string			getIP() const; */
/* 		unsigned int		getPort() const; */
/* 		std::string			get404() const; */

/* 		std::list<Location>	_locations; */
/* 	private: */
/* 		std::string			_rootPath; // dependency */
/* 		std::string			_ipAddress; // dependency */
/* 		std::string			_host; */
/* 		unsigned int		_port; // dependency */
/* 		std::string			_pathWebsite; // dependency */
/* 		std::string			_errorPage404; // dependency */
/* 		std::list< std::pair<int, std::string> >	_errorPages; */
/* 		friend class Validator; */
/* }; */

class Node;

struct ErrorPage
{
	ErrorPage( int codeArg, std::string pageArg );
	~ErrorPage();

	const int			code;
	const std::string	page;
};

class AConfig
{
	public:
		~AConfig();

		std::string	getRoot();
		ErrorPage&	getErrorPage( int errorCode );
		std::list<std::string>::const_iterator	getIndexFile();

	protected:
		bool	validatePath();
		bool	validateNumberAndRange( int min, int max );	
	private:
		AConfig();

		std::string				_root;
		std::list<ErrorPage>	_errorPages;
		std::list<std::string>	_indexFiles;
};

class Configuration : public AConfig
{
	public:
		Configuration();
		~Configuration();

		std::string		getIP();
		unsigned int	getPort();
		
	private:
		std::string				_ipAddress;
		std::string				_host; // server_name
		unsigned int			_port; // listen
		unsigned int			_clientMaxBodySize;
};

class Location : public AConfig
{
	public:
		std::string	getPath() const;
		std::string	getAlias() const;
		std::string	getCgiExtension() const;
		std::string	getCgiPath() const;
		bool		isMethodAccepted( int httpMethod ) const;
		bool		autoIndexingOn() const;	
	private:
		std::string	_path;
		std::string	_alias;
		std::string	_cgiExtension;
		std::string	_cgiPath;
		bool		_acceptedMethods[4];
		bool		_autoIndex;
};
#endif
