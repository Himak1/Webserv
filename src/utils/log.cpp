#include "log.hpp"

#include <ctime>
#include <iomanip>
#include <sys/time.h>

void printTimestamp()
{
	struct timeval time;
	gettimeofday(&time, 0);
	time_t t = time.tv_sec;
	tm* now = localtime(&t);
	char buf[100];
	strftime(buf, 100, "%Y-%m-%d %X", now);
	cout 	<< buf;
}

void log(const string message)
{
	printTimestamp();
	cout << "   " << message << endl;
}
