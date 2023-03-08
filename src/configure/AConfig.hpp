#ifndef ACONFIG_HPP
# define ACONFIG_HPP

# include <string>
# include <list>

using namespace std;

class Node;

struct ErrorPage
{
	ErrorPage( int codeArg, string pageArg );
	ErrorPage( const ErrorPage& src );
	~ErrorPage();

	const int			code;
	const string		page;
};

class AConfig
{
	public:
		~AConfig();

		string			getRoot() const;
		string			getErrorPage( int errorCode ) const;
		string			getUploadStore() const;
		unsigned int	getClientMaxBodySize() const;
		int				getRedirect() const; // returns 0 if not a redirect
		string			getRedirectURI() const; // returns empty string if not a redirect

		list<string>	indexFiles;
	protected:
		AConfig();

		bool			isDuplicate(Node* serverNode, int node_type);
		string			convertNodeToString( Node* node );
		list<string>	convertMultiNodesToStrings( Node* node );
		unsigned int	convertNodeToUInt( Node* node );
		void			convertIndexFiles( Node* node );
		void			convertErrorPage( Node* node );
		void			convertUploadStore( Node* node );
		void			convertClientMaxBodySize( Node* node );
		void			convertReturn( Node* node );
	
		bool			validatePath();
		bool			validateNumberAndRange( int min, int max );	

		string			_root;
		list<ErrorPage>	_errorPages;
		string			_uploadStore;
		unsigned int	_clientMaxBodySize;
		int				_redirectCode;
		string			_redirectURI;
};

ostream&	operator<<( ostream& o, const AConfig& config );

#endif
