#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>

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

	private:
		std::string			_pathRoot;
		std::string			_ipAddress;
		unsigned int		_port;
		std::string			_errorPage404;
};

#endif
