// C program to demonstrate use of fork() and pipe()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream> 
#include <sstream>

#define CGI_BUFSIZE 400

int main()
{
	int		fd[2];

	// save stdin and stdout so we can restore them later
	int	saveStdin = dup(STDIN_FILENO);
	int	saveStdout = dup(STDOUT_FILENO);
	
	// paht
	std::string p = "www/cgi-bin/script";
	char	*path[3];
	path[0] = &p[0];
	path[1] = NULL;

	// 
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (pipe(fd) < 0)
		exit(0);
	pid_t pid = fork();
	if (pid < 0)
		exit(0);
	if(pid == 0) {
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(path[0], path, 0);
		printf("Unknown command\n");
		exit(0);
	}
	// to do: message cannot be bigger than CGI_BUFSIZE
	char	buffer[CGI_BUFSIZE] = {0};
	read(fd[0], buffer, CGI_BUFSIZE);

	// revert stdin and stdout
	dup2(saveStdin, STDIN_FILENO);
	dup2(saveStdout, STDOUT_FILENO);
	close(fd[1]);
	std::cout << buffer << std::endl;
}
