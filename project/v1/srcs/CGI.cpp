#include "../includes/Headers.hpp"

char			**Response::create_env_tab(Request *req)
{
	char											**args_to_tab;
	std::map<std::string, std::string> 				args_to_map;
	size_t											pos;

	args_to_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	args_to_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	args_to_map["SERVER_SOFTWARE"] = "webserv";
	args_to_map["REQUEST_URI"] = req->_uri;
	args_to_map["REQUEST_METHOD"] = req->_method;
	args_to_map["REMOTE_ADDR"] = req->_client->_ip;
	args_to_map["PATH_INFO"] = req->_uri;
	// args_to_map["PATH_TRANSLATED"] = client.conf["path"];
	// args_to_map["CONTENT_LENGTH"] = std::to_string(req->_body.size());

	// if (req->_uri.find('?') != std::string::npos)
	// 	args_to_map["QUERY_STRING"] = req->_uri.substr(req->_uri.find('?') + 1);
	// else
	// 	args_to_map["QUERY_STRING"];

	// if (req->_headers.find("Content-Type") != req->_headers.end())
	// 	args_to_map["CONTENT_TYPE"] = req->_headers["Content-Type"];

	// if (client.conf.find("exec") != client.conf.end())
	// 	args_to_map["SCRIPT_NAME"] = client.conf["exec"];
	// else
	// 	args_to_map["SCRIPT_NAME"] = client.conf["path"];

	// if (client.conf["listen"].find(":") != std::string::npos)
	// {
	// 	args_to_map["SERVER_NAME"] = client.conf["listen"].substr(0, client.conf["listen"].find(":"));
	// 	args_to_map["SERVER_PORT"] = client.conf["listen"].substr(client.conf["listen"].find(":") + 1);
	// }
	// else
	// 	args_to_map["SERVER_PORT"] = client.conf["listen"];

	if (!req->_authorization.empty())
	{
		pos = req->_authorization.find(" ");
		args_to_map["AUTH_TYPE"] = req->_authorization.substr(0, pos);
		args_to_map["REMOTE_USER"] = req->_authorization.substr(pos + 1);
		args_to_map["REMOTE_IDENT"] = req->_authorization.substr(pos + 1);
	}

	// if (client.conf.find("php") != client.conf.end() && req->_uri.find(".php") != std::string::npos)
	// 	args_to_map["REDIRECT_STATUS"] = "200";

	// std::map<std::string, std::string>::iterator b = req->_headers.begin();
	// while (b != req->_headers.end())
	// {
	// 	args_to_map["HTTP_" + b->first] = b->second;
	// 	++b;
	// }

	args_to_tab = (char **)malloc(sizeof(char *) * (args_to_map.size() + 1));
	std::map<std::string, std::string>::iterator it = args_to_map.begin();
	int i = 0;
	while (it != args_to_map.end())
	{
		args_to_tab[i] = strdup((it->first + "=" + it->second).c_str());
		++i;
		++it;
	}
	args_to_tab[i] = NULL;

	return (args_to_tab);
}
