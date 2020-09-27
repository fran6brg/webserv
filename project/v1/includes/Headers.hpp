#ifndef HEADERS_HPP
#define HEADERS_HPP

/*
** Const
*/

#define RET_ERROR		0
#define RET_SUCCESS		1

#define EXIT_ERROR      1
#define EXIT_SUCCESS    0

#ifdef __linux__
# define OPEN_MAX FOPEN_MAX
#endif

/*
** Libraries
*/

#include <sys/wait.h> //linux waitpid

/*
** Other headers
*/

#include "Conf.hpp"
#include "Logger.hpp"
#include "Colors.hpp"

// https://stackoverflow.com/questions/3627941/global-variable-within-multiple-files
extern Conf g_conf;

/*
** Debug
*/

#endif
