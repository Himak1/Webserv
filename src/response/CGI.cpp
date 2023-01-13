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
CGI::CGI(class Request request, class Configuration config, string filepath)
	: _request(request), _config(config), _filepath(filepath)
{
	_filepath = _filepath.substr(0, _filepath.find("?"));
	// mogen we realpath() gebruiken?
	_path_to_script = realpath(&_filepath[0], NULL);

	_argument = new char[3];
	_argument = strcpy(_argument, "-q");
	if (_request.getExtension() == ".php") {
		_path_to_cgi = new char[PATH_TO_PHP_CGI_LENGTH + 1];
		_path_to_cgi = strcpy(_path_to_cgi, PATH_TO_PHP_CGI);
	}
	if (_request.getExtension() == ".py") {
		_path_to_cgi = new char[PATH_TO_PY_CGI_LENGTH + 1];
		_path_to_cgi = strcpy(_path_to_cgi, PATH_TO_PY_CGI);
		// _path_to_cgi = realpath("/", NULL);
	}
	_path[0] = &_path_to_cgi[0];
	_path[1] = &_path_to_script[0];
	if (_request.getExtension() == ".php")
		_path[2] = &_argument[0];
	else
		_path[2] = NULL;
	_path[3] = NULL;
	_env = createEnv();

	// cout << "_filepath = " << _filepath << endl;
	// cout << "_path_to_cgi = " << _path_to_cgi << endl;
	// cout << "_request.getExtension() = " << _request.getExtension() << endl;
	// cout << "_path_to_script = " << _path_to_script << endl;
}

// DESTRUCTOR
CGI::~CGI() {
	free(_path_to_script);
	freeEnv();
}

// PUBLIC FUNTIONS
char** 	CGI::getFormEnv() const { return _env; }

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
		perror("execve failed: ");
		cout << "Execve error" << endl;
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

// convert list to char**
char**	CGI::createEnv()
{
	list<string> env_list = _request.getEnv();

	_env = new char*[env_list.size() + 4];
	list<string>::iterator it;
	int i = 0;
	for (it = env_list.begin(); it != env_list.end(); it++) {
		_env[i] = new char[(*it).length() + 1];
		_env[i] = strcpy(_env[i], (const char*)(*it).c_str());
		i++;
	}

	string directory_listing = "directory_listing=true";
	_env[i] = new char[directory_listing.length() + 1];
	_env[i] = strcpy(_env[i], directory_listing.c_str());

	string upload_directory = "upload_directory=uploads";
	_env[++i] = new char[upload_directory.length() + 1];
	_env[i] = strcpy(_env[i], upload_directory.c_str());

	if (_request.getUploadSucces() == true) {
		string upload_succes = "upload_succes=true";
		_env[++i] = new char[upload_succes.length() + 1];
		_env[i] = strcpy(_env[i], upload_succes.c_str());
	}
	else
		_env[++i] = NULL;
	
	_env[++i] = NULL;

	return _env;
}

void	CGI::freeEnv()
{
	if (_request.getEnv().empty())
		return ;
	for (size_t i = 0; _env[i]; i++)
		delete[] _env[i];
	delete[] _env;
}
