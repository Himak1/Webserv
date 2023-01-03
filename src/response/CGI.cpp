#include "CGI.hpp"
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
	int query_start = script_name.find(".cgi") + 4;
	string query_string = script_name.substr(query_start, script_name.length()); 
	script_name = script_name.substr(0, query_start);
	string path_from_request = _config.getPathWebsite() + script_name;
	// mogen we realpath() gebruiken?
	_pathScript = realpath(&path_from_request[0], NULL);
	_path[0] = &_pathScript[0];
	_path[1] = NULL;
	
	_env = createEnv(query_string);

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
		// perror("execve :");
		log("Unknown command\n");
		exit(0);
	}
	// to do: message can currently not be bigger than CGI_BUFSIZE
	char	buffer[CGI_BUFSIZE] = {0};
	read(fd[0], buffer, CGI_BUFSIZE);

	// revert stdin and stdout
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(fd[1]);
	string message(buffer);

	return message;
}

char**	CGI::createEnv(string str)
{

	// create list of environment
	str = str.substr(str.find("?") + 1, str.length());

	int			key_size;
	int 		value_size;
	list<string>env_list;
	string entry;
	while (true) {
		key_size = str.find("=");
		entry = str.substr(0, key_size);
		str = str.substr(key_size + 1, str.length());
		value_size = str.find("&");
		entry += "=" + str.substr(0, value_size);
		str = str.substr(value_size + 1, str.length());
		env_list.push_back(entry);
		if (str.find("=") == string::npos)
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
		// cout << _env[i] << endl;
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
