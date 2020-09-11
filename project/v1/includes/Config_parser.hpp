#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <string>
# include <vector>
# include <fcntl.h>
# include <algorithm>

# include "Logger.hpp"
//# include "Utils.hpp"
# include "../srcs/get_next_line/get_next_line.hpp"

typedef struct	s_loc
{
	std::string					uri;
	std::vector<std::string>	method;
	std::string					root;
	std::string					index;
	std::string					cgi;

	s_loc()
	{
		uri = "";
		root = "";
		index = "";
		cgi = "";
	}
}				t_loc;

typedef struct	s_serv
{
	//std::string	host;
	int			port;
	std::string	error_page;
	int 		body_size;
	t_loc 		loc;

	s_serv()
	{
		port = -1;
		error_page = "";
		body_size = -1;
	}
}				t_serv;

class Config_parser
{
private:
	char *conf;
	std::vector<t_serv> serv;
	int ligne_count;

	static const std::string SERV[];

public:
	Config_parser(char *conf);
	~Config_parser();

	int setup_server();//launch parsing + init server
private:
	int read_conf();
	int parse_conf();
	// init servers() //new Server(); _servers.push_back(s1);
};

#endif
