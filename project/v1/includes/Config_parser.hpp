#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <string>
# include <vector>
# include <fcntl.h>
# include <algorithm>

# include "Server.hpp"
# include "Logger.hpp"
# include "utils_tmp.hpp"
# include "../srcs/get_next_line/get_next_line.hpp"

# define WHITE_SPACE "\t\n\v\f\r "

// Server tokens
# define _HOST "host"
# define _NAME "name"
# define _PORT "listen"
# define _ERROR_PAGE "error"

// Location tokens
# define _METHOD "method"
# define _ROOT "root"
# define _INDEX "index"
# define _CGI_PATH "cgi_path"
# define _PHP_PATH "php_path"
# define _CGI "cgi"
# define _AUTO_INDEX "auto_index"
# define _MAX_BODY "max_body"
# define _AUTH "auth"

class Conf;

extern Conf g_conf;

typedef struct	s_loc
{
	std::string					uri;
	std::vector<std::string>	method;
	std::string					root;
	std::string					index;
	std::string					cgi_path;
	std::string					php_path;
	std::string					cgi;
	int							auto_index;
	int							max_body;
	std::string					auth;
	s_loc() {
		auto_index = -1;
		max_body = -1;
	}
}				t_loc;

typedef struct	s_serv
{
	std::string			host;
	std::string			name;
	std::string			port;
	std::string			error_page;
	std::vector<t_loc> 	loc;
}				t_serv;

class Config_parser
{
private:
	char				*conf;
	int					fd;
	int					line_count;
	std::vector<t_serv> serv;

public:
	Config_parser(char *conf);
	~Config_parser();
	void setup_server(std::vector<Server *> &servers);

private:
	void parse_conf();
	void parse_server();
	void parse_location(std::vector<std::string> &token, t_serv &serv);
	void check_conf();

	void add_serv_values(std::vector<std::string> &tokens, t_serv &serv);
	void add_loc_values(std::vector<std::string> &tokens, t_loc&loc);

	void fail(const std::string &message);
	void fail_double_token(std::string &str);
	void fail_double_token(int val);
};

#endif
