#include "CGI.hpp"
#include "../utils/utils.hpp"

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
// #include <sstream>
// #include <fstream>

// CONSTRUCTOR
CGI::CGI(class Request request, class Configuration config)
	: _request(request), _config(config)
{
	std::string _path_from_request = _config.getPathWebsite() + _request.getURI();
	// mogen we realpath() gebruiken?
	_pathSscript = realpath(&_path_from_request[0], NULL);
	_path[0] = &_pathSscript[0];
	_path[1] = NULL;

	// char* data = getenv("QUERY_STRING");
	// std::cout << "GETENV " << data << std::endl;
}

// DESTRUCTOR
CGI::~CGI() {
	free(_pathSscript);
}

// PUBLIC FUNTIONS
#define CGI_BUFSIZE 1024

std::string CGI::ExecuteCGI()
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
		execve(_path[0], _path, 0);
		// perror("execve :");
		log("Unknown command\n");
		exit(0);
	}
	// to do: message cannot be bigger than CGI_BUFSIZE
	char	buffer[CGI_BUFSIZE] = {0};
	read(fd[0], buffer, CGI_BUFSIZE);

	// revert stdin and stdout
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(fd[1]);
	std::string message(buffer);
	return message;
}
