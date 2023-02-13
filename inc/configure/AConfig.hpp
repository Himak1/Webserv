#ifndef ACONFIG_HPP
# define ACONFIG_HPP

# include <string>
# include <list>

class Node;

struct ErrorPage
{
	ErrorPage( int codeArg, std::string pageArg );
	ErrorPage( const ErrorPage& src );
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

		std::list<std::string>	indexFiles;
	protected:
		AConfig();
		AConfig( const AConfig& src );

		std::string				convertNodeToString( Node* node );
		std::list<std::string>	convertMultiNodesToStrings( Node* node );
		unsigned int			convertNodeToUInt( Node* node );
	
		bool	validatePath();
		bool	validateNumberAndRange( int min, int max );	

		std::string				_root;
		std::list<ErrorPage>	_errorPages;
};

#endif
