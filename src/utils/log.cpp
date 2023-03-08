#include "log.hpp"
#include "strings.hpp"
#include <ctime>
#include <iomanip>
#include <sys/time.h>
#include <cstdlib>

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

void log_response(const string &message)
{
	string msg = safe_substr(message, 0, message.find('\n'));
	if (message.find('\n') != string::npos) {
		printTimestamp();
		cout << " < " << msg << endl;
	}
}

void log_receive(const string &message)
{
	printTimestamp();
	cout << " > " << message << endl;
}

void exitWithError(const string &errorMessage)
{
	printTimestamp();
	cout << " ERROR    " << errorMessage << endl;
	exit(1);
}
