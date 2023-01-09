#include "configuration/Configuration.hpp"

#include <iostream>

// CONSTRUCTOR
Configuration::Configuration()
{
	_pathWebsite = "www";
	_ipAddress = "0.0.0.0";
	_port = 8000;
	// _errorPage404 = "/faq.html";
	_errorPage404 = "default";
}

Configuration::Configuration(const Configuration &src) { *this = src; }

// DESTRUCTOR
Configuration::~Configuration() { }

// Overload operator
Configuration &Configuration::operator = (const Configuration &src) {
	if (this != &src) {
		_pathWebsite = src._pathWebsite;
		_ipAddress = src._ipAddress;
		_port = src._port;
		_errorPage404 = src._errorPage404;
	}
	return (*this);
}

// PUBLIC FUNTIONS
void Configuration::parseConfiguration(std::string filename)
{
	std::cout << filename << std::endl;
}

std::string			Configuration::getPathWebsite() const { return (_pathWebsite); }
std::string 		Configuration::getIP() const { return (_ipAddress); }
unsigned int		Configuration::getPort() const { return (_port); }
std::string 		Configuration::get404() const { return (_errorPage404); }

// PRIVATE FUNCTIONS
