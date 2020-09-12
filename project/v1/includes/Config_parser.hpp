#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <string>
# include <vector>
# include <fcntl.h>
# include <algorithm>

# include "Logger.hpp"
# include "utils_tmp.hpp"
# include "../srcs/get_next_line/get_next_line.hpp"

//still reachable byte when exit with wrong input file

// Server tokens
# define PORT "port"
# define ERROR_PAGE "error_page"
# define BODY_SIZE "body_size"

// Location tokens
# define METHOD "method"
# define ROOT "root"
# define INDEX "index"
# define CGI "cgi"

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
	std::vector<t_loc> loc;

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
	int		line_count;
	std::vector<t_serv> serv;

public:
	Config_parser(char *conf);
	~Config_parser();
	void setup_server();//launch parsing + init server

private:
	void parse_conf();
	void parse_server();
	void parse_location(std::vector<std::string> &token, t_serv &serv);

	void add_serv_values(std::vector<std::string> &tokens, t_serv &serv);
	void add_loc_values(std::vector<std::string> &tokens, t_loc&loc);
	// init servers() //new Server(); _servers.push_back(s1);
	void fail(const std::string &message);
};

#endif
