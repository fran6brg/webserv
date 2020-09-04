#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

std::string		get_last_modif(std::string file);
std::string		get_date(void);

#endif
