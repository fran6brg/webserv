#include "../includes/Headers.hpp"

// https://perso.liris.cnrs.fr/lionel.medini/enseignement/M1IF03/Tutoriels/Tutoriel_CGI_SSI.pdf

char			**Response::create_env_tab(Request *req)
{
	char											**args_to_tab;
	std::map<std::string, std::string> 				args_to_map;
	size_t											pos;

	args_to_map["GATEWAY_INTERFACE"] = "CGI/1.1"; // version des spécifications CGI utilisées par le serveur
	args_to_map["SERVER_PROTOCOL"] = "HTTP/1.1"; // protocole et version de la requête en cours de traitement
	args_to_map["SERVER_SOFTWARE"] = g_conf._webserv; // nom et version du démon HTTP
	args_to_map["REQUEST_URI"] = req->_uri;
	args_to_map["REQUEST_METHOD"] = req->_method; // méthode associée à la requête en cours de traitement
	// args_to_map["REMOTE_HOST"] = ?; // nom de la machine d'où vient la requête 
	args_to_map["REMOTE_ADDR"] = req->_client->_ip; // adresse IP de la machine d'où vient la requête
	args_to_map["PATH_INFO"] = req->_uri; // chaîne entre SCRIPT_PATH et QUERY_STRING dans l'URL
	// args_to_map["PATH_TRANSLATED"] = client.conf["path"];
	args_to_map["CONTENT_LENGTH"] = std::to_string(req->_body.size()); // longueur des données véhiculées dans la requête (POST)

	if (req->_uri.find('?') != std::string::npos)
		args_to_map["QUERY_STRING"] = req->_uri.substr(req->_uri.find('?') + 1); // données transmises au CGI via l'URL (GET)
	else
		args_to_map["QUERY_STRING"]; // données transmises au CGI via l'URL (GET)

	// if (req->_headers.find("Content-Type") != req->_headers.end())
		args_to_map["CONTENT_TYPE"] = map_to_string(req->_content_type, ';'); // type MIME des données véhiculées dans la requête

	// if (client.conf.find("exec") != client.conf.end())
	// 	args_to_map["SCRIPT_NAME"] = client.conf["exec"]; // chemin du CGI à partir de la racine du serveur HTTP
	// else
	// 	args_to_map["SCRIPT_NAME"] = client.conf["path"]; // chemin du CGI à partir de la racine du serveur HTTP

	// if (client.conf["listen"].find(":") != std::string::npos)
	// {
		// args_to_map["SERVER_NAME"] = client.conf["listen"].substr(0, client.conf["listen"].find(":"));
		args_to_map["SERVER_NAME"] = req->_client->_server->_name; // nom ou adresse IP de la machine serveur HTTP
		// args_to_map["SERVER_PORT"] = client.conf["listen"].substr(client.conf["listen"].find(":") + 1); // numéro du port (TCP) vers lequel la requête a été envoyée
		args_to_map["SERVER_PORT"] = req->_client->_server->_port; // numéro du port (TCP) vers lequel la requête a été envoyée
	// }
	// else
	// 	args_to_map["SERVER_PORT"] = client.conf["listen"]; // numéro du port (TCP) vers lequel la requête a été envoyée

	if (!req->_authorization.empty())
	{
		pos = req->_authorization.find(" ");
		args_to_map["AUTH_TYPE"] = req->_authorization.substr(0, pos); // méthode d'authentification de l'utilisateur s'il y a lieu
		args_to_map["REMOTE_USER"] = req->_authorization.substr(pos + 1); // si authentification, nom de l'utilisateur associé à la requête
		args_to_map["REMOTE_IDENT"] = req->_authorization.substr(pos + 1); // login de connexion de l'utilisateur (pas souvent supporté)
	}

	// if (client.conf.find("php") != client.conf.end() && req->_uri.find(".php") != std::string::npos)
	// 	args_to_map["REDIRECT_STATUS"] = "200";

	// std::map<std::string, std::string>::iterator b = req->_headers.begin();
	// while (b != req->_headers.end())
	// {
	// 	args_to_map["HTTP_" + b->first] = b->second; // une variable pour chaque champ contenu dans l'en-tête HTTP
	// 	++b;
	// }

	args_to_tab = (char **)malloc(sizeof(char *) * (args_to_map.size() + 1));
	std::map<std::string, std::string>::iterator it = args_to_map.begin();
	int i = -1;
	while (it != args_to_map.end())
	{
		args_to_tab[++i] = strdup((it->first + "=" + it->second).c_str());
		++it;
	}
	args_to_tab[i] = NULL;

	return (args_to_tab);
}
