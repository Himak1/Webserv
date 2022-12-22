#include "CGI.hpp"

#include <unistd.h>
#include <iostream>
#include <cstring>
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

#define CGI_BUFSIZE 3000
// PUBLIC FUNTIONS
void CGI::ExecuteCGI()
{
	// char	*path[2];
	// path[0] = &_pathSscript[0];
	// path[1] = NULL;

	// execve(path[0], path, NULL);
	std::cout << "CGI " << std::endl;

	// pid_t		pid;
	// char		**env = NULL;
	// std::string	newBody;

	// try {
	// 	env = this->_getEnvAsCstrArray();
	// }
	// catch (std::bad_alloc &e) {
	// 	std::cerr << RED << e.what() << RESET << std::endl;
	// }

	// // SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	// int saveStdin = dup(STDIN_FILENO);
	// int saveStdout = dup(STDOUT_FILENO);

	// FILE	*fIn = tmpfile();
	// FILE	*fOut = tmpfile();
	// long	fdIn = fileno(fIn);
	// long	fdOut = fileno(fOut);
	// int		ret = 1;

	// // write(fdIn, _body.c_str(), _body.size());
	// lseek(fdIn, 0, SEEK_SET);

	// pid = fork();

	// if (pid == -1)
	// 	return ;
	// else if (!pid) {
	// 	char * const * nll = NULL;

	// 	dup2(fdIn, STDIN_FILENO);
	// 	dup2(fdOut, STDOUT_FILENO);
	// 	execve(_pathSscript.c_str(), nll, env);
	// 	write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	// }
	// else {
	// 	char	buffer[CGI_BUFSIZE] = {0};

	// 	// waitpid(-1, NULL, 0);
	// 	lseek(fdOut, 0, SEEK_SET);

	// 	ret = 1;
	// 	while (ret > 0) {
	// 		memset(buffer, 0, CGI_BUFSIZE);
	// 		ret = read(fdOut, buffer, CGI_BUFSIZE - 1);
	// 		newBody += buffer;
	// 	}
	// }

	// dup2(saveStdin, STDIN_FILENO);
	// dup2(saveStdout, STDOUT_FILENO);
	// fclose(fIn);
	// fclose(fOut);
	// close(fdIn);
	// close(fdOut);
	// close(saveStdin);
	// close(saveStdout);

	// // for (size_t i = 0; env[i]; i++)
	// // 	delete[] env[i];
	// // delete[] env;

	// if (!pid)
	// 	exit(0);

	// std::cout << newBody << std::endl;
	// return (newBody);
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
