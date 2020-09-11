#include "../includes/Config_parser.hpp"

Config_parser::Config_parser(char *conf) : conf(conf)
{
	ligne_count = 0;
}

Config_parser::~Config_parser()
{
}

const std::string Config_parser::SERV[] =
{
    "port",
    "error_page",
    "body_size",
};

void Config_parser::fail(const std::string &message)
{
	ERROR_RET("Config_parser: " + message);
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
	while ((ret = get_next_line(fd, &cline)) && ++ligne_count)
	{
		line = cline;
		free(cline);
		LOG_WRT(Logger::DEBUG, "Conf  : " + line + "[" + std::to_string(ligne_count) + "]");
		line.erase(std::remove_if(line.begin(), line.end(), utils_tmp::isspace), line.end());
		if (line.length() == 7 && line.compare("server{") == 0)
			parse_server();
		else if (line.length() != 0)
			fail("bad syntax ligne: " + std::to_string(ligne_count));	
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

	while ((ret = get_next_line(fd, &cline)) && ++ligne_count)
	{
		line = cline;
		free(cline);
		LOG_WRT(Logger::DEBUG, "Server: " + line + "[" + std::to_string(ligne_count) + "]");
		std::vector<std::string> tokens= utils_tmp::split_string(line);
		if (tokens.size() == 1 && tokens[0] == "}")
			return ;
		else if (tokens[0][0] == '#')
			continue ;
		else if (tokens[0] == "location")
			parse_location(line);
		else if (tokens.size() != 2)
			fail(std::to_string(ligne_count) + " arguments is to much");
		else
		{
			LOG_WRT(Logger::DEBUG, "good" );
		}
		
		//size_t i = -1;
		//while (++i < tokens.size())
		//	LOG_WRT(Logger::DEBUG, "Server: SPLIT=" + tokens[i]);
		//split string into word
		//check tokens
		//if tokens =location
		//	lauch location_parser()
		//else
		//	create a fuction for each tokens use function ptr et string association
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
}

void Config_parser::parse_location(std::string &loc_line)
{
	int			ret;
	char		*cline;
	std::string	line;

	while ((ret = get_next_line(fd, &cline)) && ++ligne_count)
	{
		line = cline;
		free(cline);
		LOG_WRT(Logger::DEBUG, "Location: " + line + "[" + std::to_string(ligne_count) + "]");
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
}