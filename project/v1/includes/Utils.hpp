#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "Request.hpp"

std::string		get_last_modif(std::string file);
std::string		get_date(void);
std::string		get_content_type(std::string file);
std::string     get_location_header(Request *req);
std::string     map_to_string(std::map<int, std::string> map, char delim);
std::string     vector_to_string(std::vector<std::string> map, char delim);
void            displayMap(std::map<std::string, std::string> map);
int	    		is_php(std::string file);

int             set_laguage(Request *req);
int             set_charset(Request *req);
void            unset_extension(Request *req);

#endif
