#ifndef UTILS_TMP_HPP
# define UTILS_TMP_HPP

# include <string>
# include <sys/time.h>	//get_date
# include <string.h>	//get_date
# include <vector>

// add a std::to_string

class utils_tmp
{
public:
	static std::string	get_date(void);
	static int isspace(int c);
	static std::vector<std::string> split_string(std::string &str);
};

#endif
