#ifndef LOG_H
# define LOG_H

# include <iostream>
# include <sstream>

using namespace std;

void	log_receive(const string &message);
void	log_response(const string &message);
void    exitWithError(const string &errorMessage);

#endif
