#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Response::Response(void)/*:
    _http_version(std::string("")),
    _reason_phrase(std::string("")),
    _allow(std::string("")),
    _last_modified(std::string("")),
    _location(std::string("")),
    _date(std::string("")),
    _retry_after(std::string("")),
    _server(std::string("")),
    _to_send(std::string(""))*/
{
    init();
}


/*
** private class methods
*/

void Response::init(void)
{
    // Status Line
    _http_version.clear();
    _status_code = 0;
    _reason_phrase.clear();
    // Request Headers, dans l'ordre du sujet
    _allow.clear();
    _content_language.clear();
    _content_length = 0; // est-ce qu'on peut avoir un content_length de 0 ?
    _content_location.clear();
    _content_type.clear();
    _last_modified.clear();
    _location.clear();
    _date.clear();
    _retry_after.clear();
    _server.clear();
    _transfer_encoding.clear();
    _www_authenticate.clear();
    // Request body
    _body.clear();
    // Content to write in fd
    _to_send.clear();
}

int Response::concat_to_send(void)
{
    std::stringstream ss;
    size_t i;
    
    // 1. Status Line
    ss << _http_version << " ";
    ss << _status_code << " ";
    ss << _reason_phrase << "\r\n";

    // 2. Request Headers, dans l'ordre du sujet
    // if (!_content_language.empty())     { ss << "content_language: " << _content_language << "\r\n"; }
    // if (!_date.empty())                 { ss << "Date: " << _date << "\r\n"; }
    if (_content_length >= 0)
        ss << "Content-Length: " << _content_length << "\r\n";
    // if (!_content_location.empty())     { ss << "content_location: " << _content_location << "\r\n"; }
    if (!_allow.empty())                { ss << "allow: " << _allow << "\r\n"; }
    // if (!_content_type.empty())         { ss << "Content-Type: " << _content_type << "\r\n"; }
    if (!_last_modified.empty())        { ss << "Last-Modified: " << _last_modified << "\r\n"; }
    if (!_location.empty() && _status_code == 201)  { ss << "Location: " << _location << "\r\n"; }
    if (!_date.empty())                 { ss << "Date: " << _date << "\r\n"; }
    if (!_retry_after.empty())          { ss << "Retry-After: " << _retry_after << "\r\n"; }
    if (!_server.empty())               { ss << "Server: " << _server << "\r\n"; }
    // if (!_transfer_encoding.empty())    { ss << "transfer_encoding: " << _transfer_encoding << "\r\n"; }
    // if (!_www_authenticate.empty())     { ss << "www_authenticate: " << _www_authenticate << "\r\n"; }
    if (!_content_type.empty())
	{
		ss << "Content-Type:";
        i = 0;
    	while (i < _content_type.size())
    	{
    	    ss << " " << _content_type[i];
    	    if (i + 1 < _content_type.size())
    	        ss << ";";
            i++;
    	}
    	ss << "\r\n";
	}

    // 3. Request body
    // ss << "\r\n\r\n"; // double or simple CRLF after the headers ?
    ss << "\r\n"; // CRLF et non pas \n\n https://stackoverflow.com/questions/29131727/http-header-and-message-body-separator-clarification
    if (!_body.empty())                 { ss << _body; }
    // Convert
    _to_send = ss.str();
    return (1);
}

/*
** public class methods
*/

int Response::format_to_send(Request *req)
{
    // Status Line
    _http_version = "HTTP/1.1";
    _reason_phrase = code_to_reason[_status_code];
    _content_length = _body.length(); // https://stackoverflow.com/questions/13821263/should-newline-be-included-in-http-response-content-length
	
	// Response headers, dans l'ordre du sujet
    _date = get_date();
    // LOG_WRT(Logger::DEBUG, "format_to_send: " + _date);

    _server = g_conf._webserv;
	if ((req->_method == "GET" || req->_method == "HEAD" || req->_method == "PUT" || req->_method == "POST")
    && (_status_code == OK_200 || _status_code == CREATED_201) && _content_type[0] == "")
		 _content_type[0] = get_content_type(req->_file);
	if (_status_code == CREATED_201)
		_location = get_location_header(req);	
	_content_language.clear();
    _content_location.clear();
    _retry_after.clear();
    _transfer_encoding.clear();
    _www_authenticate.clear();
	// Response body
	if (req->_method == "HEAD")
		_body.clear();
    concat_to_send();
    return (1);
}

void		Response::handle_response(Request *req)
{
	// TEMPORAIRE
	// req->_method = "TRACE";
    if (bad_request(req))
		return ;
	else if (method_not_allowed(req))
		return ;
	else if (req->_method == "GET" || req->_method == "HEAD")
		get(req);
	else if (req->_method == "POST")
		post(req);
	else if (req->_method == "PUT")
		put(req);
	else if (req->_method == "DELETE")
		ft_delete(req);
	else if (req->_method == "OPTIONS")
		option(req);
	else if (req->_method == "TRACE")
		trace(req);
	else if (req->_method == "CONNECT")
		connect(req);
}

int				Response::method_not_allowed(Request *req)
{
	// Comparaison de la methode demande avec les methodes autorise dans la location
	for (std::size_t i = 0; i < (req->_location->_method).size(); ++i)
	{
		LOG_WRT(Logger::DEBUG, "test if " + (req->_location->_method)[i] + " == " + req->_method + "\n");
		if ((req->_location->_method)[i] == req->_method)
			return (0);
	}
	LOG_WRT(Logger::DEBUG, "METHOD_NOT_ALLOWED_405\n");
    _status_code = METHOD_NOT_ALLOWED_405;
	_allow = vector_to_string(req->_location->_method, ',');
	std::string path = "./www/old/error/405.html";
    std::ifstream error405(path);
    std::string buffer((std::istreambuf_iterator<char>(error405)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}

int		Response::accepted_method(Request *req)
{
    return (req->_method == "GET"
         || req->_method == "HEAD"
         || req->_method == "PUT"
         || req->_method == "POST"
         || req->_method == "DELETE"
         || req->_method == "OPTIONS"
		 || req->_method == "TRACE"
		 || req->_method == "CONNECT");
}

int				Response::bad_request(Request *req)
{
	if (accepted_method(req))
        return (0);
    _status_code = BAD_REQUEST_400;
	std::string path = "./www/old/error/400.html";
    std::ifstream error400(path);
    std::string buffer((std::istreambuf_iterator<char>(error400)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}

int				Response::not_found(Request *req)
{
	(void)req;
    _status_code = NOT_FOUND_404;
	std::string path = "./www/old/error/404.html";
    std::ifstream error404(path);
    std::string buffer((std::istreambuf_iterator<char>(error404)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}
