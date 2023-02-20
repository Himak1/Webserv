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
CGI::CGI(class Request request, class Location* location, string filepath, int clientMaxBodySize)
	: _request(request), _location(location), _filepath(filepath), _clientMaxBodySize(clientMaxBodySize)
{
	_buffer = (char *)calloc(_clientMaxBodySize, sizeof(char));
	_env = new char*[_request.getEnv().size() + 4];

	createPath();
	createEnv();
}

// DESTRUCTOR
CGI::~CGI() {
	free(_buffer);
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
		cout << "<!doctype html><html lang=\"en\"><head><title>"
			<< "500 Internal Server Error\n"
			<< "</title></head><body><center><h1>"
			<< "500 Internal Server Error\n"
			<< "</h1></center></body></html>" << endl;
		exit(0);
	}
	close(fd[1]);

	int message_size = read(fd[0], _buffer, _clientMaxBodySize);
	if (message_size >= _clientMaxBodySize)
		return "<!doctype html><html lang=\"en\"><head><title>" \
				"413 Request Entity Too Large\n" \
				"</title></head><body><center><h1>" \
				"413 Request Entity Too Large\n" \
				"</h1></center></body></html>";
	return _buffer;
}

// PRIVATE FUNCTIONS
void	CGI::createPath()
{
	_filepath = _filepath.substr(0, _filepath.find("?"));
	_path_to_script = realpath(&_filepath[0], NULL);

	int path_length = (*_location).getCgiPath().size();
	char *_path_to_cgi = new char[path_length + 1];
	_path_to_cgi = strcpy(_path_to_cgi, (*_location).getCgiPath().c_str());
	_path[0] = &_path_to_cgi[0];
	_path[1] = &_path_to_script[0];
	_path[2] = NULL;
}

void	CGI::createEnv()
{
	map<string, string>	env_list = _request.getEnv();
	map<string, string>::iterator it;
	int i = 0;
	for (it = env_list.begin(); it != env_list.end(); it++) {
		addToEnv((*it).first + "=" + (*it).second, i);
		i++;
	}

	if (_location->autoIndexingOn())
		addToEnv("directory_listing=true", i);
	else
		addToEnv("directory_listing=false", i);

	string temp_define_2 = UPLOAD_FOLDER;
	addToEnv("upload_directory=" + temp_define_2, ++i);

	if (_request.getUploadSucces() == true) addToEnv("upload_succes=true", ++i);
	
	_env[++i] = NULL;
}

void	CGI::addToEnv(string value, int i)
{
	_env[i] = new char[value.length() + 1];
	strcpy(_env[i], value.c_str());
}

void	CGI::freeEnv()
{
	if (_request.getEnv().empty())
		return ;
	for (size_t i = 0; _env[i]; i++)
		delete[] _env[i];
	delete[] _env;
}
