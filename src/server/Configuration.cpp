#include "Configuration.hpp"

#include <iostream>

// CONSTRUCTOR
Configuration::Configuration()
{
	this->_pathWebsite = "www";
	this->_ipAddress = "0.0.0.0";
	this->_port = 8000;
	this->_errorPage404 = "default";
}

Configuration::Configuration(const Configuration &src) { *this = src; }

// DESTRUCTOR
Configuration::~Configuration() { }

// Overload operator
Configuration &Configuration::operator = (const Configuration &src) {
	if (this != &src) {
		this->_pathWebsite = src._pathWebsite;
		this->_ipAddress = src._ipAddress;
		this->_port = src._port;
		this->_errorPage404 = src._errorPage404;
	}
	return (*this);
}

// PUBLIC FUNTIONS
void Configuration::parseConfiguration(std::string filename)
{
	std::cout << filename << std::endl;
}

std::string			Configuration::getPathWebsite() const { return (this->_pathWebsite); }
std::string 		Configuration::getIP() const { return (this->_ipAddress); }
unsigned int		Configuration::getPort() const { return (this->_port); }
std::string 		Configuration::get404() const { return (this->_errorPage404); }

// PRIVATE FUNCTIONS
