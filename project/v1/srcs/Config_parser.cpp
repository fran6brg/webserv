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


int Config_parser::setup_server()
{
	if (!parse_conf())
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int Config_parser::parse_conf()
{
	int			ret;
	int			fd;
	char		*cline;
	std::string	line;

	if (conf == NULL)
		return (EXIT_FAILURE);
	if ((fd = open(conf, O_RDONLY)) == -1)
		return (ERROR_RET("Config_parser: fail to open config file"));
	while ((ret = get_next_line(fd, &cline)) && ++ligne_count)
	{
		line = cline;
		free(cline);
		line.erase(std::remove_if(line.begin(), line.end(), utils_tmp::isspace), line.end());
		if (line.length() == 7 && line.compare("server{") == 0)
			;
		else if (line.length() != 0)
			return (ERROR_RET("Config_parser: bad syntax ligne: " + std::to_string(ligne_count)));
		std::cout << line  << "->" << line.length() << std::endl;
	}
	if (ret < 0)
		return (ERROR_RET("Config_parser: fail to read config file"));
	
	return (EXIT_SUCCESS);
}