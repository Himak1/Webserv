#ifndef ACONFIG_HPP
# define ACONFIG_HPP

# include <string>
# include <list>

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

		std::list<std::string>	indexFiles;
	protected:
		AConfig();

		std::string		convertNodeToString( Node* node );
		unsigned int	convertNodeToUInt( Node* node );
	

		bool	validatePath();
		bool	validateNumberAndRange( int min, int max );	
	private:

		std::string				_root;
		std::list<ErrorPage>	_errorPages;
};

#endif
