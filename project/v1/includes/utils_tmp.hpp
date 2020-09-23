#ifndef UTILS_TMP_HPP
# define UTILS_TMP_HPP

# include <string>
# include <sys/time.h>	//get_date
# include <string.h>	//get_date
# include <vector>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include "../srcs/get_next_line/get_next_line.hpp"
# include "Logger.hpp"
// add a std::to_string


class utils_tmp
{
public:
	static std::string	get_date(void);
	static int	isspace(int c);
	static std::vector<std::string> split_string(std::string &str, std::string set);
	static bool file_exists(const char *filename);
	static int read_file(int fd, std::string file, std::string &buff);
	static std::string extract_body(std::string &buff);
	static int hexa_to_dec(const char *hexVal);
	static void remove_return(std::string &str);
};

#endif
