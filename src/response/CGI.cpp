#include "CGI.hpp"
#include "Response.hpp"
#include "../utils/log.hpp"

#include <list>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>


// CONSTRUCTOR
CGI::CGI(class Request request, class Configuration config)
	: _request(request), _config(config)
{
	string script_name = _request.getURI();
	script_name = script_name.substr(0, script_name.find(".cgi") + 4);
	string path_from_request = _config.getPathWebsite() + script_name;
	// mogen we realpath() gebruiken?
	_pathScript = realpath(&path_from_request[0], NULL);
	_path[0] = &_pathScript[0];
	_path[1] = NULL;
	
	_env = createEnv();

	// char* data = getenv("QUERY_STRING");
	// cout << "GETENV " << data << endl;
}

// DESTRUCTOR
CGI::~CGI() {
	free(_pathScript);
	freeEnv();
}

// PUBLIC FUNTIONS
char** 	CGI::getFormEnv() const { return _env; }

#define CGI_BUFSIZE 1024
#include <sstream>
string CGI::ExecuteCGI()
{
	// save stdin and stdout so we can restore them later
	int	saveStdin = dup(STDIN_FILENO);
	int	saveStdout = dup(STDOUT_FILENO);
	
	// execute script
	int		fd[2];
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (pipe(fd) < 0)
		return "Error: pipe";
	pid_t pid = fork();
	if (pid < 0)
		return "Error: fork";
	if(pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(_path[0], _path, _env);

		
		ostringstream ss;
		ss	<< "<!DOCTYPE html><html lang=\"en\"><head><title>"
			<< "400 Bad Request\n"
			<< "</title>"
			<< "</head><body><center><h1>"
			<< "400 Bad Request\n"
			<< "</h1></center></body></html>";
		return ss.str();

		// string content = ss.str();
		// cout << _request.getHTTPVersion() << " ";
		// cout << "400 Bad Request\n";
		// cout << "Content-Type: text/html; charset=utf-8\n";
		// cout << "Content-Length: ";
		// cout << content.size() << "\n\n";
		// cout << content;
		exit(0);
	}
	// TO DO: message can currently not be bigger than CGI_BUFSIZE
	char	buffer[CGI_BUFSIZE] = {0};
	read(fd[0], buffer, CGI_BUFSIZE);

	// revert stdin and stdout
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(fd[1]);
	string message(buffer);

	return message;
}

// TO DO: opschonen
char**	CGI::createEnv()
{

	// create list of environment
	string uri = _request.getURI();
	string query = uri.substr(uri.find("?") + 1, uri.length());

	int			key_size;
	int 		value_size;
	list<string>env_list;
	string entry;
	while (true) {
		key_size = query.find("=");
		entry = query.substr(0, key_size);
		query = query.substr(key_size + 1, query.length());
		value_size = query.find("&");
		entry += "=" + query.substr(0, value_size);
		query = query.substr(value_size + 1, query.length());
		env_list.push_back(entry);
		if (query.find("=") == string::npos)
			break ;
	}

	// convert list to char**
	if (env_list.empty())
		return NULL;

	_env = new char*[env_list.size() + 1];
	list<string>::iterator it;
	int i = 0;
	for (it = env_list.begin(); it != env_list.end(); it++) {
		_env[i] = new char[(*it).length() + 1];
		_env[i] = strcpy(_env[i], (const char*)(*it).c_str());
		cout << _env[i] << endl;
		i++;
	}
	_env[i] = NULL;
	return _env;
}

void	CGI::freeEnv()
{
	for (size_t i = 0; _env[i]; i++)
		delete[] _env[i];
	delete[] _env;
}
