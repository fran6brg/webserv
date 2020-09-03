#ifndef HEADERS_HPP
#define HEADERS_HPP

/*
** CONST
*/

#define RET_ERROR		0
#define RET_SUCCESS		1

#define EXIT_ERROR      1
#define EXIT_SUCCESS    0

/*
** Libraries
*/

/*
** Other headers
*/

#include "Conf.hpp"

// https://stackoverflow.com/questions/3627941/global-variable-within-multiple-files
extern Conf g_conf;

#endif
