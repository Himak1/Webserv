#include "log.hpp"

#include <ctime>
#include <iomanip>
#include <sys/time.h>

void printTimestamp()
{
	struct timeval time;
	gettimeofday(&time, 0);
	std::time_t t = time.tv_sec;
	std::tm* now = std::localtime(&t);
	char buf[100];
	std::strftime(buf, 100, "%Y-%m-%d %X", now);
	std::cout 	<< buf;
}

void log(const std::string message)
{
	printTimestamp();
	std::cout << "   " << message << std::endl;
}
