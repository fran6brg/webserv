#ifndef UTILS_TMP_HPP
# define UTILS_TMP_HPP

# include <string>
# include <sys/time.h>
# include <string.h>
# include <vector>
# include <map>
# include <sstream>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include "../srcs/get_next_line/get_next_line.hpp"
# include "Logger.hpp"


class utils_tmp
{
public:
	static std::string	get_date(void);
	static int	isspace(int c);
	static std::vector<std::string> split_string(std::string &str, std::string set);
	static bool file_exists(const char *filename);
	static int get_buffer(std::string file, std::string &buff);
	static int extract_body(std::string &buff);
	static int hexa_to_dec(const char *hexVal);
	static void remove_return(std::string &str);
	static size_t getSecondsDiff(std::string complete_time);
	static void	ft_getline(std::string &b, std::string &line);
	static std::vector<std::string> split(const std::string& str, char delim);
	static std::string trim(const std::string& str);
	static void print_map(std::stringstream &ss1, std::map<int, std::string> map);
	static void free_strtab(char ***tab);
	static int is_valide_methods(std::string &meth);
};

#endif
