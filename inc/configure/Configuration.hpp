
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include <utility>

class Location;

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

		std::list<Location>	_locations;
	private:
		std::string			_rootPath; // dependency
		std::string			_ipAddress; // dependency
		std::string			_host;
		unsigned int		_port; // dependency
		std::list< std::pair<int, std::string> >	_errorPages;
};

#endif
