#include "../includes/Config_parser.hpp"

Config_parser::Config_parser(char *conf) : conf(conf)
{
	line_count = 0;
}

Config_parser::~Config_parser()
{
}

void Config_parser::fail(const std::string &message)
{
	ERROR_RET("Config_parser: " + message);
	if (fd != -1)
		close(fd);
	exit(EXIT_FAILURE);
}

void Config_parser::fail_double_token(std::string &str)
{
	if (str.length() != 0)
		fail("Double token [" + std::to_string(line_count) + "]");
}
void Config_parser::fail_double_token(int val)
{
	if (val != -1)
		fail("Double token [" + std::to_string(line_count) + "]");
}

void Config_parser::setup_server(std::vector<Server *> &servers)
{
	parse_conf();
	check_conf();

	for (size_t i = 0; i < serv.size(); ++i)
	{
		Server *server = new Server(serv[i].name, stoi(serv[i].port), serv[i].host, serv[i].error_page);
		servers.push_back(server);

		LOG_WRT(Logger::DEBUG, "SERVER " + std::to_string(i));
		LOG_WRT(Logger::DEBUG, "host       = " + serv[i].host);
		LOG_WRT(Logger::DEBUG, "name       = " + serv[i].name);
		LOG_WRT(Logger::DEBUG, "port       = " + serv[i].port);
		LOG_WRT(Logger::DEBUG, "error_page = " + serv[i].error_page);
		for (size_t y = 0; y < serv[i].loc.size(); ++y)
		{
			Location *location = new Location(	serv[i].loc[y].uri, serv[i].loc[y].root, serv[i].loc[y].index,
												serv[i].loc[y].method, serv[i].loc[y].cgi_path, serv[i].loc[y].php_path,
												serv[i].loc[y].cgi, serv[i].loc[y].auto_index, serv[i].loc[y].max_body,
												serv[i].loc[y].auth);

			server->_locations.push_back(location);

			LOG_WRT(Logger::DEBUG, "LOCATION " + std::to_string(y));
			LOG_WRT(Logger::DEBUG, "	uri    = " + serv[i].loc[y].uri);
			if (serv[i].loc[y].method.size())
				LOG_WRT(Logger::DEBUG, "	method");
			for (size_t z = 0; z < serv[i].loc[y].method.size(); z++)
				LOG_WRT(Logger::DEBUG, "    " + serv[i].loc[y].method[z]);
			LOG_WRT(Logger::DEBUG, "	root       = " + serv[i].loc[y].root);
			LOG_WRT(Logger::DEBUG, "	index      = " + serv[i].loc[y].index);
			LOG_WRT(Logger::DEBUG, "	cgi_path   = " + serv[i].loc[y].cgi_path);
			LOG_WRT(Logger::DEBUG, "	php_path   = " + serv[i].loc[y].php_path);
			LOG_WRT(Logger::DEBUG, "	cgi        = " + serv[i].loc[y].cgi);
			LOG_WRT(Logger::DEBUG, "	auto_index = " + std::to_string(serv[i].loc[y].auto_index));
			LOG_WRT(Logger::DEBUG, "	max_body   = " + std::to_string(serv[i].loc[y].max_body));
			LOG_WRT(Logger::DEBUG, "	auth       = " + serv[i].loc[y].auth);

		}
		LOG_WRT(Logger::DEBUG, "");
	}
}

void Config_parser::check_conf()
{
	for (size_t i = 0; i < serv.size(); ++i)
	{
		for (size_t j = 0; j < serv.size(); ++j)
			if (i != j && serv[i].port == serv[j].port)
				fail("Duplicate ports");

		int is_default_root = 0;
		for (size_t y = 0; y < serv[i].loc.size(); ++y)
		{
			if (serv[i].loc[y].uri == "/")
				is_default_root = 1;
			for (size_t z = 0; z < serv[i].loc[y].method.size(); z++)
				if (!utils_tmp::is_valide_methods(serv[i].loc[y].method[z]))
					fail("Not valide methode");
			if (serv[i].loc[y].auto_index < -1 || serv[i].loc[y].auto_index > 1)
				fail("auto_index unvalid (on = 1 / off = 0)");
		}
		if (!is_default_root)
			fail("Server need default location root");
	}
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
		cline = NULL;
		line.erase(std::remove_if(line.begin(), line.end(), utils_tmp::isspace), line.end());
		if (line.length() == 7 && line.compare("server{") == 0)
			parse_server();
		else if (line[0] == '#')
			continue ;
		else if (line.length() != 0)
			fail("bad syntax [" + std::to_string(line_count) + "]");	
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
	t_serv		new_serv;

	while ((ret = get_next_line(fd, &cline)) && ++line_count)
	{
		line = cline;
		free(cline);
		cline = NULL;
		std::vector<std::string> tokens= utils_tmp::split_string(line, WHITE_SPACE);
		if (tokens.size() == 1 && tokens[0] == "}")
		{
			serv.push_back(new_serv);
			return ;
		}
		else if (tokens.size() == 0 || tokens[0][0] == '#')
			continue ;
		else if (tokens[0] == "location")
			parse_location(tokens, new_serv);
		else 
			add_serv_values(tokens, new_serv);
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
	fail("Bracket not close [" + std::to_string(line_count) + "]");
}

void Config_parser::parse_location(std::vector<std::string> &token, t_serv &serv)
{
	int			ret;
	char		*cline;
	std::string	line;
	t_loc		new_loc;

	// init
	new_loc.max_body = -1;
	if (!(token.size() == 3 && token[2][0] == '{')
		&& !(token.size() == 2 && token[1][token[1].length() - 1] == '{'))
		fail("Bad location: " + line + "[" + std::to_string(line_count) + "]");
	new_loc.uri = token[1];
	while ((ret = get_next_line(fd, &cline)) && ++line_count)
	{
		line = cline;
		free(cline);
		cline = NULL;
		std::vector<std::string> tokens= utils_tmp::split_string(line, WHITE_SPACE);
		if (tokens.size() == 1 && tokens[0] == "}")
		{
			serv.loc.push_back(new_loc);
			return ;
		}
		else if (tokens.size() == 0 || tokens[0][0] == '#')
			continue ;
		else
			add_loc_values(tokens, new_loc);
	}
	if (cline)
		free(cline);
	if (ret < 0)
		fail("fail to read config file");
	fail("Bracket not close [" + std::to_string(line_count) + "]");
}

void Config_parser::add_serv_values(std::vector<std::string> &tokens, t_serv &serv)
{
	if (tokens.size() == 1)
		fail("no arguments given [" + std::to_string(line_count) + "]");
	if (tokens.size() > 2 && tokens[2][0] != '#')
		fail("to many arguments [" + std::to_string(line_count) + "]");

	if (tokens[0] == _HOST)
	{
		fail_double_token(serv.host);
		serv.host = tokens[1];
	}
	else if (tokens[0] == _NAME)
	{
		fail_double_token(serv.name);
		serv.name = tokens[1];
	}
	else if (tokens[0] == _PORT)
	{
		fail_double_token(serv.port);
		serv.port = tokens[1];
	}
	else if (tokens[0] == _ERROR_PAGE)
	{
		fail_double_token(serv.error_page);
		serv.error_page = tokens[1];
	}
	else
		fail("Token invalid (" + tokens[0] + ") [" + std::to_string(line_count) + "]");
}

void Config_parser::add_loc_values(std::vector<std::string> &tokens, t_loc &loc)
{
	if (tokens.size() == 1)
		fail("no arguments given [" + std::to_string(line_count) + "]");

	if (tokens[0] == _METHOD)
	{
		if (loc.method.size() != 0)
			fail("Double token [" + std::to_string(line_count) + "]");
		for (size_t i = 1; i < tokens.size(); ++i)
			loc.method.push_back(tokens[i]);
	}
	else
	{
		if (tokens.size() > 2 && tokens[2][0] != '#')
			fail("too many arguments [" + std::to_string(line_count) + "]");
		if (tokens[0] == _ROOT)
		{
			fail_double_token(loc.root);
			loc.root = tokens[1];
		}
		else if (tokens[0] == _INDEX)
		{
			fail_double_token(loc.index);
			loc.index = tokens[1];
		}
		else if (tokens[0] == _CGI_PATH)
		{
			fail_double_token(loc.cgi_path);
			loc.cgi_path = tokens[1];
		}
		else if (tokens[0] == _PHP_PATH)
		{
			fail_double_token(loc.php_path);
			loc.php_path = tokens[1];
		}
		else if (tokens[0] == _CGI)
		{
			fail_double_token(loc.cgi);
			loc.cgi = tokens[1];
		}
		else if (tokens[0] == _AUTO_INDEX)
		{
			fail_double_token(loc.auto_index);
			loc.auto_index = stoi(tokens[1]);
		}
		else if (tokens[0] == _MAX_BODY)
		{
			fail_double_token(loc.max_body);
			loc.max_body = stoi(tokens[1]);
		}
		else if (tokens[0] == _AUTH)
		{
			fail_double_token(loc.auth);
			loc.auth = tokens[1];
		}	
		else
			fail("Token invalid (" + tokens[0] + ") [" + std::to_string(line_count) + "]");
	}
}
