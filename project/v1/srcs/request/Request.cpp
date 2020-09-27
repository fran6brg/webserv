#include "../../includes/Headers.hpp"

/*
** Constructors / Destructors
*/

Request::Request(void)
{
    init();
}

void    Request::init(void)
{
    // Request Line
    _method.clear();
    _uri.clear();
    _http_version.clear();
    // Request Headers, dans l'ordre du sujet
    _accept_charset.clear();
    _accept_language.clear();
    _authorization.clear();
    _content_language.clear();
    _content_length = -1;
    _content_location.clear();
    _content_type.clear();
    _date.clear();
    _host.clear();
    _referer.clear();
    _user_agent.clear();
    // Request body
    _text_body.clear();
    _body.clear();
    _transfer_encoding.clear();
    // Other headers not mentionned in the subject
    _keep_alive.clear();

    // autres variables qui ne sont pas des headers
    _body_length = -1;
    _body_type = -1;
    _saved_error = -1;
    _file.clear();
    _location = NULL;
    
    _secret_header.clear();
}

/*
** for CGI:
*/

std::map<std::string, std::string> Request::headers_to_map(void)
{
    std::map<std::string, std::string> ret;

    /*
    ** Request Headers, dans l'ordre du sujet
    */ 

   ret["Accept-Charset"] = map_to_string(_accept_charset, ';');
   ret["Accept-Language"] = map_to_string(_accept_language, ',');
   ret["Authorization"] = _authorization;
   ret["Content-Language"] = map_to_string(_content_language, ',');
   ret["Content-Length"] = std::to_string(_content_length);
   ret["Content-Location"] = _content_location;
   ret["Content-Type"] = _content_type;
   ret["Date"] = _date;
   ret["Host"] = _host;
   ret["Referer"] = _referer;
   ret["User-Agent"] = _user_agent;
   if (!_secret_header.empty())
       ret["X-Secret-Header-For-Test"] = _secret_header;

   return (ret);
}

/*
** Reset before waiting for new call
*/

void Request::reset(void)
{
    init();
}
