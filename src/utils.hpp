#ifndef UTILS_H
# define UTILS_H

#include <iostream>

std::string	readStream(std::string filename);
int			getStreamSize(std::string filename);
void		printTimestamp();
void		log(const std::string message);

#endif