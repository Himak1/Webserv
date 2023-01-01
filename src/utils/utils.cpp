#include "utils.hpp"

#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sys/time.h>

std::string readStream(std::string filename) {
	std::basic_ifstream<char> input_stream(filename.c_str());
	if (!input_stream.is_open()) {
		input_stream.close();
		return "fileNotFound";
	}

	std::ostringstream ss;
	std::string line;
	while (std::getline(input_stream, line))
		ss << line << std::endl;
	return ss.str();
}

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
