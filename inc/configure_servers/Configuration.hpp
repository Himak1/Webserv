
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
		std::string			getPathWebsite() const;
		std::string			getIP() const;
		unsigned int		getPort() const;
		std::string			get404() const;

	private:
		std::string			_pathWebsite;
		std::string			_ipAddress;
		unsigned int		_port;
		std::string			_errorPage404;
};

int	exampleFunc(int number);

#endif
