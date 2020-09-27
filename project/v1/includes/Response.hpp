#ifndef RESPONSE_HPP
#define RESPONSE_HPP

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
#include <fstream>
#include <sstream>
#include <unistd.h>

/*
** Headers
*/

#include "Utils.hpp"
#include "Request.hpp"
#include "Code.hpp"

class Client;

/*
** Class
*/

class Response
{
	/*
	** member variables
	*/

	private:
		// 

	protected:
		//
	
	public:

		Client 		*_client;

		/*
		** Status Line
		*/

		std::string					_http_version;
		int 						_status_code;
		std::string					_reason_phrase;

		/*
		** Response Headers, dans l'ordre du sujet
		*/

		// - Contexte de réponse
		std::string  				_allow; // liste les méthodes supportées par une ressource. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Allow
		// - Informations sur le corps du message:
		std::map<int, std::string>  _content_language; // pour décrire quels langages sont destinés au public, de sorte que cela permette à l'utilisateur de se différencier en fonction de la langue préférée des utilisateurs. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Language
		int                         _content_length; // indique la taille en octets (exprimée en base 10) du corps de la réponse envoyée au client. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Length
		std::map<int, std::string>  _content_location; // indicates an alternate location for the returned data. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Location
		std::map<int, std::string>	_content_type; // sert à indiquer le type MIME de la ressource. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Type
		// - Conditionnels:
		std::string                 _last_modified; //  la date et l'heure à laquelle le serveur d'origine pense que la ressource a été modifiée pour la dernière fois. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Last-Modified
		// - Redirection:
		std::string 				_location; // indique l'URL vers laquelle rediriger une page. Il a un sens seulement lorsqu'il est servi avec une réponse d'état 3xx (redirection) ou 201 (créé). https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Location
		// - Autres:
		std::string 				_date; // la date et l'heure d'origine du message. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Date
		int			 				_retry_after; // indicates how long the user agent should wait before making a follow-up request. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Retry-After
		// - Contexte de réponse:
		std::string 				_server; // contient des informations à propos du système (ou sous-système) en place sur le serveur qui s'occupe de la requête. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Serveur
		// - Contexte de réponse:
		std::map<int, std::string> 	_transfer_encoding; // specifies the form of encoding used to safely transfer the payload body to the user. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Transfer-Encoding
		// - Authentification:
		std::map<int, std::string> 	_www_authenticate; // définit la méthode d'authentification qui doit être utilisé pour obtenir l'accès à une ressource. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/WWW-Authenticate

		/*
		** Response body
		*/

		std::string					_body;

		/*
		** Response body
		*/

		std::string 				_to_send;

		/*
		** Other headers (not mentionned in the subject but useful to consider)
		*/

		std::string			 		_keep_alive; // 18 allows the sender to hint about how the connection may be used to set a timeout and a maximum amount of requests. https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Keep-Alive

		/*
		** Other
		*/

		size_t						_bytes_send;

	/*
	** methods
	*/

	private:
		void init(void);
		void reset(void);
		int concat_to_send(void);

	protected:
		//

	public:
		Response(void);
		// ~Response(void);

		int 			format_to_send(Request *req);

		void			handle_response(Request *req);
		
		void			get(Request *req);
		void			post(Request *req);
		void			put(Request *req);
		void			ft_delete(Request *req);
		void			option(Request *req);
		void            trace(Request *req);
		void			connect(Request *req);

		int				bad_request(Request *req);
		int				accepted_method(Request *req);
		int				method_not_allowed(Request *req);
		int				unauthorized(Request *req);
		int				request_entity_too_large(Request *req);
		int				not_found(Request *req);
		int             service_unavailable(Request *req);

		char			**create_env_tab(Request *req);
		void			ft_cgi(Request *req);
		void			get_cgi_ret(Request *req);
	/*
	** friends
	*/

	friend class Client;
};

#endif
