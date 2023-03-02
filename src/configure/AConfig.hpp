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

		std::string				getRoot() const;
		std::string				getErrorPage( int errorCode ) const;
		std::string				getUploadStore() const;
		unsigned int			getClientMaxBodySize() const;
		int						getRedirect() const; // returns 0 if not a redirect
		std::string				getRedirectURI() const; // returns empty string if not a redirect

		std::list<std::string>	indexFiles;
	protected:
		AConfig();

		std::string				convertNodeToString( Node* node );
		std::list<std::string>	convertMultiNodesToStrings( Node* node );
		unsigned int			convertNodeToUInt( Node* node );
		void					convertIndexFiles( Node* node );
		void					convertErrorPage( Node* node );
		void					convertUploadStore( Node* node );
		void					convertClientMaxBodySize( Node* node );
		void					convertReturn( Node* node );
	
		bool					validatePath();
		bool					validateNumberAndRange( int min, int max );	

		std::string				_root;
		std::list<ErrorPage>	_errorPages;
		std::string				_uploadStore;
		unsigned int			_clientMaxBodySize;
		int						_redirectCode;
		std::string				_redirectURI;
};

std::ostream&	operator<<( std::ostream& o, const AConfig& config );

#endif
