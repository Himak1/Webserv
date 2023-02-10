#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

class Validator
{
	public:
		Validator( Node* Server );
		~Validator();
	private:
		Node*	server;
		
};

/*
 * BaseClass Configure()
	CreateServerConfig();
		ValidateRoot();
		ValidateErrorPage();
		ValidateListen();
		ValidateServerName();
		Vli
	CreateLocation();
		ValidateRoot();
		ValidateErrorPage();
		ValidateAlias();
		ValidateCgiPass();
		Validate
		ValidateReturn();
*/

#endif
