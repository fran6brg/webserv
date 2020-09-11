#ifndef UTILS_TMP_HPP
# define UTILS_TMP_HPP

# include <string>
# include <sys/time.h>	//get_date
# include <string.h>	//get_date

class utils_tmp
{
public:
	static std::string	get_date(void);
	static int isspace(int c);
};

#endif
