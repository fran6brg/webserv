#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <string>
# include <vector>

typedef struct	s_loc
{
	std::string					uri;
	std::vector<std::string>	method;
	std::string					root;
	std::string					index;
	std::string					cgi;

}				t_loc;

typedef struct	s_serv
{
	//std::string	host;
	int			port;
	std::string	error_page;
	int 		body_size;
	t_loc 		loc;
}				t_serv;
/*
const std::string Config_parser::PRIORITY_NAMES[] =
{
    "DEBUG",
    "INFO",
    "ERROR"
};*/

class Config_parser
{
private:
	char *conf;
	std::vector<t_serv> serv;

public:
	Config_parser(char *conf);
	~Config_parser();

	int setup_server();//launch parsing + init server
private:
	int parse_server();
	// init servers() //new Server(); _servers.push_back(s1);
};

#endif
