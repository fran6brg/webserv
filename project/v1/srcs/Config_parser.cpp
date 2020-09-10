#include "../includes/Config_parser.hpp"

Config_parser::Config_parser(char *conf) : conf(conf)
{
}

Config_parser::~Config_parser()
{
}

int Config_parser::setup_server()
{
	if (!parse_server())
		return (0);
	return (1);
}

int Config_parser::parse_server()
{
	
	return (1);
}