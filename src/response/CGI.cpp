#include "CGI.hpp"
#include "Response.hpp"
#include "../utils/log.hpp"
#include "../configure/Location.hpp"

#include <list>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// CONSTRUCTOR
// CGI::CGI(class Request request, class Configuration config, string filepath)
CGI::CGI(class Request request, class Location& location, string filepath)
	: _request(request), _location(location), _filepath(filepath)
{
	_filepath = _filepath.substr(0, _filepath.find("?"));
	// mogen we realpath() gebruiken? Is onderdeel van C POSIX library, waar bijv. unistd.h en stdlib.h ook onder vallen
	_path_to_script = realpath(&_filepath[0], NULL);

	_argument = new char[3];
	_argument = strcpy(_argument, "-q");

	int path_length = _location.getCgiPath().size();
	char *_path_to_cgi = new char[path_length + 1];
	_path_to_cgi = strcpy(_path_to_cgi, _location.getCgiPath().c_str());
	_path[0] = &_path_to_cgi[0];
	_path[1] = &_path_to_script[0];
	_path[2] = NULL;
	_env = createEnv();

	// cout << "_filepath = " << _filepath << endl;
	// cout << "_path_to_cgi = " << _location.getCgiPath().c_str() << endl;
	// cout << "_path_to_script = " << _path_to_script << endl;
	// cout << "_request.getExtension() = " << _request.getExtension() << endl;
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
	int		fd[2];
	fd[0] = 0;
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
		cout << "<!DOCTYPE html><html lang=\"en\"><head><title>"
		<< "500 Internal Server Error\n"
		<< "</title></head><body><center><h1>"
		<< "500 Internal Server Error\n"
		<< "</h1></center></body></html>" << endl;
		exit(0);
	}
	// TO DO: message can currently not be bigger than CGI_BUFSIZE
	char	buffer[CGI_BUFSIZE] = {0};
	read(fd[0], buffer, CGI_BUFSIZE);
	close(fd[1]);

	return buffer;
}

// convert list to char**
char**	CGI::createEnv()
{
	map<string, string>	env_list = _request.getEnv();

	_env = new char*[env_list.size() + 4];
	map<string, string>::iterator it;
	int i = 0;
	for (it = env_list.begin(); it != env_list.end(); it++) {
		string env_var = (*it).first + "=" + (*it).second;
		int length = env_var.length() + 2;
		_env[i] = new char[length];
		_env[i] = strncpy(_env[i], (const char*)env_var.c_str(), length);
		i++;
	}

	string temp_define_1 = DIRECTORY_LISTING;
	string directory_listing = "directory_listing=" + temp_define_1;
	_env[i] = new char[directory_listing.length() + 1];
	_env[i] = strcpy(_env[i], directory_listing.c_str());

	string temp_define_2 = UPLOAD_FOLDER;
	string upload_directory = "upload_directory=" + temp_define_2;
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
