#include "../includes/Config_parser.hpp"

Config_parser::Config_parser(char *conf) : conf(conf)
{
	line_count = 0;
}

Config_parser::~Config_parser()
{
}
/*
funcServ Config_parser::ServTab[] =
{
	"port",
	"error_page",
	"body_size"
}*/

void Config_parser::fail(const std::string &message)
{
	ERROR_RET("Config_parser: " + message);
	close(fd);//check if need to be close
	//free
	exit(EXIT_FAILURE);
}


void Config_parser::setup_server()
{
	parse_conf();
}

void Config_parser::parse_conf()
{
	int			ret;
	char		*cline;
	std::string	line;

	if (conf == NULL)
		fail("unvalib file");
	if ((fd = open(conf, O_RDONLY)) == -1)
		fail("fail to open config file");
	while ((ret = get_next_line(fd, &cline)) && ++line_count)
	{
		line = cline;
		free(cline);
		LOG_WRT(Logger::DEBUG, "Conf  : " + line + "[" + std::to_string(line_count) + "]");
		line.erase(std::remove_if(line.begin(), line.end(), utils_tmp::isspace), line.end());
		if (line.length() == 7 && line.compare("server{") == 0)
			parse_server();
		else if (line.length() != 0)
			fail("bad syntax line: " + std::to_string(line_count));	
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
}

void Config_parser::parse_server()
{
	int			ret;
	char		*cline;
	std::string	line;

	serv.push_back();
	while ((ret = get_next_line(fd, &cline)) && ++line_count)
	{
		line = cline;
		//erase_semicol(line); if { or } return
		free(cline);
		LOG_WRT(Logger::DEBUG, "Server: " + line + "[" + std::to_string(line_count) + "]");
		std::vector<std::string> tokens= utils_tmp::split_string(line);
		if (tokens.size() == 1 && tokens[0] == "}")
			break ;
		else if (tokens.size() == 0 || tokens[0][0] == '#')
			continue ;
		else if (tokens[0] == "location")
			parse_location(line, serv.back());
		else 
			add_serv_values(tokens, serv.back());
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
}

void Config_parser::parse_location(std::vector<std::string> &token, t_serv &serv)
{
	int			ret;
	char		*cline;
	std::string	line;

	if (!(token.size() == 3 && token[2][0] == '{')
		&& !(token.size() == 2 && token[1][token[1].length() - 1] == '{'))
		fail("Bad location: " + line + "[" + std::to_string(line_count) + "]");
	serv.loc.push_back();
	serv.loc.back().uri = token[1];
	while ((ret = get_next_line(fd, &cline)) && ++line_count)
	{
		line = cline;
		free(cline);
		LOG_WRT(Logger::DEBUG, "Location: " + line + "[" + std::to_string(line_count) + "]");
		std::vector<std::string> tokens= utils_tmp::split_string(line);
		if (tokens.size() == 1 && tokens[0] == "}")
			break ;
		else if (tokens.size() == 0 || tokens[0][0] == '#')
			continue ;
		else
		{
			//LOG_WRT(Logger::DEBUG, "good" );
		}
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
}

void add_serv_values(std::vector<std::string> &tokens, t_serv &serv)
{
	if (tokens.size() == 1)
		fail("no arguments given " + line + "[" + std::to_string(line_count) + "]");
	if (tokens.size() > 2 && tokens[2][0] != '#')
		fail("to many arguments " + line + "[" + std::to_string(line_count) + "]");
	
	if (tokens[0] == PORT)
	{
		serv.port = tokens[1];
	}
	else if (tokens[0] == ERROR_PAGE)
	{
		serv.error_page = tokens[1];
	}
	else if (tokens[0] == BODY_SIZE)
	{
		serv.body_size = tokens[1];
	}
	else
		fail("Token invalid " + line + "[" + std::to_string(line_count) + "]");
}
