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

/*
** Headers
*/

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
        // Request Line
        std::string _method;
        std::string _uri;
        std::string _http_version;
        
        // Request Headers, dans l'ordre du sujet
            // - Négociation de contenu:
        std::map<int, std::string> _accept_charset; // indique le jeu de caractères que le client est capable de comprendre. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept-Charset
        std::map<int, std::string> _accept_language; // indique quelles sont les langues que le client est capable de comprendre, et quelle variante locale est préférée https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept-Language
            // - Authentification:
        std::map<int, std::pair<std::string, std::string>> _authorization; // contient les identifiants permettant l'authentification d'un utilisateur https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Authorization
            // - informations sur le corps du message:
        std::map<int, std::string> _content_language; // pour décrire quels langages sont destinés au public, de sorte que cela permette à l'utilisateur de se différencier en fonction de la langue préférée des utilisateurs. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Language
        int _content_length; // indique la taille en octets (exprimée en base 10) du corps de la réponse envoyée au client. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Length
        std::map<int, std::string> _content_location; // indicates an alternate location for the returned data. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Location
        std::map<int, std::string> _content_type; // sert à indiquer le type MIME de la ressource. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Content-Type
            // - Autres:
        std::string _date; // la date et l'heure d'origine du message. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Date
            // - Contexte de requête:
        std::string _host; // spécifie le nom de domaine du serveur https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Host
        std::string _referer; // l'adresse de la page web précédente à partir de laquelle un lien a été suivi pour demander la page courante. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Referer
        std::string _user_agent; // string that lets servers and network peers identify the application, operating system, vendor, and/or version of the requesting user agent. https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/User-Agent

        // Request Body
        std::map<int, std::pair<std::string, std::string>> _body;

    /*
    ** methods
    */

    private:
        // 

    protected:
        //

    public:
        Request();


    /*
    ** friends
    */

    friend class Conf;
    friend class Server;
};

#endif