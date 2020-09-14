#ifndef REQUEST_HPP
#define REQUEST_HPP

/*
** Doc
*/

// https://www.ntu.edu.sg/home/ehchua/programming/webprogramming/HTTP_Basics.html
	// see 'HTTP Request and Response Messages '
// https://developer.mozilla.org/fr/docs/Web/HTTP/Headers

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <map>
#include <utility> // std::pair, std::make_pair
#include <sstream>
#include <sys/stat.h> 

/*
** Headers
*/

#include "Server.hpp"
#include "Location.hpp"
// #include "Client.hpp"

class Client;

/*
** Class
*/

class Request
{
	/*
	** member variables
	*/

	private:
		// 

	protected:
		//
	
	public:
		Client				*_client;
		Location			*_location;
		std::string			_buffer;
		std::string 		_file;
		std::string			_file_name;
		int					_body_length;
	
		/*
		** Request Line
		*/

		std::string 										_method; // 1
		std::string 										_uri; // 2
		std::string 										_http_version; // 3
	
		std::string											_query;	
		/*
		** Request Headers, dans l'ordre du sujet
		*/ 

		// - Négociation de contenu:
		std::map<int, std::string> 							_accept_charset; // 4 indique le jeu de caractères que le client est capable de comprendre. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept-Charset
		// { 1: "utf-8", 2: "iso-8859-1;q=0.5" }
		
		std::map<int, std::string> 							_accept_language; // 5 indique quelles sont les langues que le client est capable de comprendre, et quelle variante locale est préférée https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept-Language
		// - Authentification:
		std::string 										_authorization; // 6 contient les identifiants permettant l'authentification d'un utilisateur https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Authorization
		// - informations sur le corps du message:
		std::map<int, std::string> 							_content_language; // 7 pour décrire quels langages sont destinés au public, de sorte que cela permette à l'utilisateur de se différencier en fonction de la langue préférée des utilisateurs. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Language
		int			 										_content_length; // 8 indique la taille en octets (exprimée en base 10) du corps de la réponse envoyée au client. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Length
		std::string				 							_content_location; // 9 indicates an alternate location for the returned data. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Location
		std::map<int, std::string> 							_content_type; // 10 sert à indiquer le type MIME de la ressource. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Type
		// - Autres:
		std::string 										_date; // 11 la date et l'heure d'origine du message. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Date
		// - Contexte de requête:
		std::string 										_host; // 12 spécifie le nom de domaine du serveur https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Host
		std::string 										_referer; // 13 l'adresse de la page web précédente à partir de laquelle un lien a été suivi pour demander la page courante. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Referer
		std::string 										_user_agent; // 14 string that lets servers and network peers identify the application, operating system, vendor, and/or version of the requesting user agent. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/User-Agent
		// Not a request header according to https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Transfer-Encoding but needed
		std::string			 								_transfer_encoding; // specifies the form of encoding used to safely transfer the payload body to the user. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Transfer-Encoding

		/*
		** Request body
		*/

		std::map<int, std::pair<std::string, std::string> > _body; // 15

	/*
	** methods
	*/

	private:
		void fill_request(std::string key, std::string value);

	protected:
		//

	public:
		Request();
		void 				init();

		std::map<std::string, std::string>	headers_to_map(void);
    	int 				parse_request_line(void);
    	int 				parse_headers(void);
    	int 				parse_body(void);
    	int 				parse_chunked_body(void);
		void				parse_query_string();
		int					get_location(std::string *uri, std::vector<Location*> location);
		int					parse_filename(std::vector<Location*> location);
		int 				parse(std::vector<Location*> location);
		void 				display(void);

	/*
	** friends
	*/

	friend class Client;
};

#endif
