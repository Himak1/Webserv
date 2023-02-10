#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>

class Configuration
{
	public:
		Configuration();
		Configuration(const Configuration &src);
		~Configuration();
		Configuration &operator = (const Configuration &src);

		void       			parseConfiguration(std::string filename);
		std::string			getPathRoot() const;
		std::string			getIP() const;
		unsigned int		getPort() const;
		std::list<std::string>		indexFiles;

	private:
		std::string			_pathRoot;
		std::string			_ipAddress;
		unsigned int		_port;
		std::string			_errorPage404;
};

#endif
