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
	char	*conf;
	int		fd;
	int		ligne_count;
	std::vector<t_serv> serv;

	static const std::string SERV[];

public:
	Config_parser(char *conf);
	~Config_parser();

	void setup_server();//launch parsing + init server
private:
	void parse_conf();
	void parse_server();
	void parse_location(std::string &loc_line);
	// init servers() //new Server(); _servers.push_back(s1);
	void fail(const std::string &message);
};

#endif
