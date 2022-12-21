#include "CGI.hpp"

#include <unistd.h>
#include <iostream>
// #include <sstream>
// #include <fstream>

// CONSTRUCTOR
CGI::CGI(class HTTPRequest request, class Configuration config)
	: _request(request), _config(config)
{
	_pathSscript = "www/cgi-bin/script.cgi";
}

// DESTRUCTOR
CGI::~CGI() { }

// PUBLIC FUNTIONS
void CGI::ExecuteCGI()
{
	char	*path[2];
	path[0] = &_pathSscript[0];
	path[1] = NULL;

	// pid_t pid;
	// pid = fork();

	// int fd[2];
	// dup2(fd[0], 1);
	// dup2(fd[1], 2);
	// close(fd[0]);

	execve(path[0], path, NULL);
	std::cout << "CGI " << std::endl;

	// std::basic_ifstream<char> input_stream(_filename);
	// if (!input_stream.is_open()) {
	// 	input_stream.close();
	// 	return (fileNotFound());
	// }

	// input_stream.seekg(0, std::ios::end);
	// std::streampos size = input_stream.tellg();
	// input_stream.seekg(0, std::ios::beg);

	// std::ostringstream ss;
	// ss	<< streamStatus(status)
	// 	<< size << "\n\n";

	// std::string line;
	// while (std::getline(input_stream, line))
	// 	ss << line << std::endl;

	// input_stream.close();
	// return ss.str();
}

// PRIVATE FUNCTIONS
// static void	create_pipe_and_execute_commands()
// {
	// int		fd[2];
	// std::string	path[2] = {0}; 
	// path[0]= "www/cgi-bin/script.cgi";

	// dup2(fd[0], STDIN_FILENO);
	// close(fd[0]);
	// if (pipe(fd) == -1)
	// 	exit (EXIT_FAILURE);
	// int pid = fork();
	// if (pid < 0)
	// 	exit (EXIT_FAILURE);
	// if (pid == 0) {
		// dup2(fd[1], STDOUT_FILENO);
		// close(fd[1]);
		// execve(path[0], path, NULL);
		// if (execve(path, NULL, NULL) == -1)
			// handle_perror(commands);
	// }
	// close(fd[1]);
// }
