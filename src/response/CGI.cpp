#include "CGI.hpp"
#include "Response.hpp"
#include "../utils/log.hpp"
#include "../utils/fileHandling.hpp"
#include "../configure/Location.hpp"

#include <list>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fstream>

// CONSTRUCTOR
CGI::CGI(const class Request& request, class Location* location, string filepath, int clientMaxBodySize)
	: _request(request), _location(location), _filepath(filepath), _clientMaxBodySize(clientMaxBodySize)
{
	_allocation_has_failed = false;

	_buffer = (char *)calloc(_clientMaxBodySize, sizeof(char));
	if (!_buffer)
		_allocation_has_failed = true;

	if (!_allocation_has_failed) {
		try {
			_env = new char*[_request.getEnv().size() + 3];
		} catch (bad_alloc&) {
			_allocation_has_failed = true;
		}
	}

	createPath();
	createEnv();
}

// DESTRUCTOR
CGI::~CGI() {
	free(_buffer);
	delete[] _path[0];
	free(_path[1]);
	freeEnv();
}

// PUBLIC FUNTIONS
char** 	CGI::getFormEnv() const { return _env; }
#include "../utils/strings.hpp"

string CGI::ExecuteCGI()
{
	if (_allocation_has_failed)
		return INTERNAL_SERVER_ERROR_MSG;

	if ((*_location).getCgiExtension() == ".php") {
		if (hasInfiniteLoop("(true)"))
			return BAD_GATEWAY_MSG;
		if (hasInfiniteLoop("(1)"))
			return BAD_GATEWAY_MSG;

		string errors = pipeAndFork(STDERR_FILENO);
		if (errors.size() > 1)
			return BAD_GATEWAY_MSG;
	}

	return pipeAndFork(STDOUT_FILENO);
}

// PRIVATE FUNCTIONS
string	CGI::pipeAndFork(int output)
{
		
						//// ALARM
	int		fd[2] = {0};

	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);

	if (pipe(fd) < 0)
		return "Error: pipe";
	pid_t pid = fork();
	if (pid < 0)
		return "Error: pipe";


	if(pid == 0) {
		dup2(fd[1], output);
		close(fd[1]);
		alarm(5);				// wacht 5 seconden en stuurt dan een SIGALRM in het child proces

		if (output == STDERR_FILENO)
			close(STDOUT_FILENO);
		execve(_path[0], _path, _env);
		cout << INTERNAL_SERVER_ERROR_MSG << endl;
		exit(0);
	}
	close(fd[1]);
	int wait_stat;
	
	waitpid(pid, &wait_stat, 0);
	if (WIFEXITED(wait_stat))
		cout << "exceve 'normal' exit? with code " << WEXITSTATUS(wait_stat) << endl;
	else if (WIFSIGNALED(wait_stat)) {		// hier vangen we SIGALRM op 
		cout << "exceve timeoud; exit with signal code " << WTERMSIG(wait_stat) << endl;
		return INTERNAL_SERVER_ERROR_MSG;
	}

	int message_size = read(fd[0], _buffer, _clientMaxBodySize);
	if (message_size >= static_cast<int>(_clientMaxBodySize))
		return REQUEST_ENTITY_TOO_LARGE_MSG;

	return _buffer;


								// ORGINEEL

    // if (pid == 0) {
    //     // Child process
    //     execve("script.sh", NULL, NULL);
    //     exit(0);
    // } else if (pid > 0) {
    //     // Parent process
    //      // Set a timeout of 10 seconds
    //     int status;
    //     waitpid(pid, &status, 0);
    //     if (WIFEXITED(status)) {
    //         std::cout << "Script exited with status " << WEXITSTATUS(status) << std::endl;
    //     } else if (WIFSIGNALED(status)) {
    //         std::cout << "Script was terminated by signal " << WTERMSIG(status) << std::endl;
    //     }
    // } else {
    //     std::cerr << "Failed to fork" << std::endl;
    //     exit(1);
    // }


	// if(pid == 0) {
	// 	dup2(fd[1], output);
	// 	close(fd[1]);
	// 	if (output == STDERR_FILENO)
	// 		close(STDOUT_FILENO);
	// 	execve(_path[0], _path, _env);
	// 	cout << INTERNAL_SERVER_ERROR_MSG << endl;
	// 	exit(0);
	// }
	// close(fd[1]);

	// int message_size = read(fd[0], _buffer, _clientMaxBodySize);
	// if (message_size >= static_cast<int>(_clientMaxBodySize))
	// 	return REQUEST_ENTITY_TOO_LARGE_MSG;

	// return _buffer;
}

bool CGI::hasInfiniteLoop(string condition)
{
	string code = streamPhpFileDataToString(_filepath);
	remove(code.begin(), code.end(), ' ');

	string::size_type whilePos = code.find("while");
	while (whilePos != string::npos) {
		string::size_type truePos = code.find(condition, whilePos);
		if (truePos == string::npos) {
			whilePos = code.find("while", whilePos + 1);
			continue;
		}

		string::size_type closeBracePos = code.find("}", truePos);
		if (closeBracePos == string::npos) {
			whilePos = code.find("while", whilePos + 1);
			continue;
		}

		string loopBody = code.substr(truePos, closeBracePos - truePos + 1);
		if (loopBody.find("break") == string::npos &&
			loopBody.find("return") == string::npos) {
			return true;
		}

		whilePos = code.find("while", whilePos + 1);
	}
	return false;
}

void	CGI::createPath()
{
	_filepath = _filepath.substr(0, _filepath.find("?"));
	char *path_to_script = realpath(&_filepath[0], NULL);

	int path_length = (*_location).getCgiPath().size();
	if (!_allocation_has_failed) {
		try {
			char *path_to_cgi = new char[path_length + 1];
			path_to_cgi = strcpy(path_to_cgi, (*_location).getCgiPath().c_str());
			_path[0] = &path_to_cgi[0];
			_path[1] = &path_to_script[0];
			_path[2] = NULL;
		} catch (bad_alloc&) {
			_allocation_has_failed = true;
		}
	}
}

void	CGI::createEnv()
{
	map<string, string>	env_list = _request.getEnv();
	map<string, string>::iterator it;
	int i = 0;
	for (it = env_list.begin(); it != env_list.end(); it++)
		addToEnv((*it).first + "=" + (*it).second, i++);

	if (_location->autoIndexingOn())
		addToEnv("directory_listing=true", i);
	else
		addToEnv("directory_listing=false", i);

	addToEnv("upload_directory=" + _location->getUploadStore(), ++i);

	_env[++i] = NULL;
}

void	CGI::addToEnv(string value, int i)
{
	if (!_allocation_has_failed) {
		try {
			_env[i] = new char[value.length() + 1];
			strcpy(_env[i], value.c_str());
		} catch (bad_alloc&) {
			_allocation_has_failed = true;
		}
	}
}

void	CGI::freeEnv()
{
	if (_request.getEnv().empty())
		return ;
	for (size_t i = 0; _env[i]; i++)
		delete[] _env[i];
	delete[] _env;
}
