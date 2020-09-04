#include "../includes/Headers.hpp"
/*
** constructors / destructors
*/

Response::Response(void)
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
    _content_length = 0;
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
    size_t i = 0;
    
    // Status Line
    ss << _http_version << " ";
    ss << _status_code << " ";
    ss << _reason_phrase << "\r\n";
    // Request Headers, dans l'ordre du sujet
    // if (!_content_language.empty())     { ss << "content_language: " << _content_language << "\r\n"; }
    if (_content_length > 0)            { ss << "Content-Length: " << _content_length << "\r\n"; }
    // if (!_content_location.empty())     { ss << "content_location: " << _content_location << "\r\n"; }

    ss << "Content-Type:";
    while (i < _content_type.size())
    {
        ss << " " << _content_type[i++];
        if (i + 1 < _content_type.size())
            ss << ";";
    }
    ss << "\r\n";
    
    if (!_last_modified.empty())        { ss << "Last-Modified: " << _last_modified << "\r\n"; }
    if (!_location.empty())             { ss << "Location: " << _location << "\r\n"; }
    if (!_date.empty())                 { ss << "Date: " << _date << "\r\n"; }
    if (!_retry_after.empty())          { ss << "Retry-After: " << _retry_after << "\r\n"; }
    if (!_server.empty())               { ss << "Server: " << _server << "\r\n"; }
    // if (!_transfer_encoding.empty())    { ss << "transfer_encoding: " << _transfer_encoding << "\r\n"; }
    // if (!_www_authenticate.empty())     { ss << "www_authenticate: " << _www_authenticate << "\r\n"; }
    // Request body
    ss << "\n\n";
    if (!_body.empty())                 { ss << _body << " "; }
    // Convert
    _to_send = ss.str();
    // _to_send = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 44\n\n<html><body><h1>It works!</h1></body></html>";
    return (1);
}

/*
** public class methods
*/

int Response::format_to_send(Request *req)
{
    // Status Line
    _http_version = "HTTP/1.1";
//	_reason_phrase = ; ---- > acceder au bon message en fonction du _status_code
    
	// Request Headers, dans l'ordre du sujet
    _allow.clear();
    _content_language.clear();
    _content_length = _body.size();
    _content_location.clear();
    _content_type[1] = "text/html";
	_last_modified = get_last_modif(req->_file);
    _location.clear();
    _date = get_date();
    _retry_after.clear();
    _server.clear();
    _transfer_encoding.clear();
    _www_authenticate.clear();
	
	if (req->_method == "HEAD")
		_body.clear();
    concat_to_send();
    return (1);
}

void		Response::handle_response(Request *req)
{
	if (req->_method == "GET" || req->_method == "HEAD")
		get(req);
	else if (req->_method == "POST")
		post(req);
	else if (req->_method == "PUT")
		put(req);
	else if (req->_method == "DELETE")
		ft_delete(req);
	else if (req->_method == "OPTION")
		option(req);
	else
        method_not_allowed(req);
}

void			Response::get(Request *req)
{
	std::ifstream file(req->_file);
	if (file.good())
	{
		std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		// gerer le cas ou la requete definie content-size (copier dans le body que les size premier caracteres ?)
		_body = buffer;
		_status_code = 200;
		_reason_phrase = code_to_reason[_status_code];
	}
	else
	{
		// ERREUR 404
		std::ifstream error404("www/404.html");
		std::string buffer((std::istreambuf_iterator<char>(error404)), std::istreambuf_iterator<char>());
		_body = buffer;
		_status_code = 404;
		_reason_phrase = code_to_reason[_status_code];
	}
}

void			Response::post(Request *req)
{
	(void)req;
}

void			Response::put(Request *req)
{
	(void)req;
}

void			Response::ft_delete(Request *req)
{
	int	ret;
	std::ifstream file(req->_file);
	if (file.good())
	{
		ret = remove(req->_file.c_str());
		if (!ret)
		{
			_status_code = 200;
			_reason_phrase = code_to_reason[_status_code];
		}
		else
		{
			// ERREUR 202
			_status_code = 202;
			_reason_phrase = code_to_reason[_status_code];
		}
	}
	else
	{
		// ERREUR 204
		_status_code = 204;
		_reason_phrase = code_to_reason[_status_code];
	}
}

void			Response::option(Request *req)
{
	(void)req;
}

void			Response::method_not_allowed(Request *req)
{
    (void)req;
    std::ifstream error405("www/405.html"); // method not allowed
    std::string buffer((std::istreambuf_iterator<char>(error405)), std::istreambuf_iterator<char>());

    _body = buffer;
    _status_code = 405;
    _reason_phrase = code_to_reason[_status_code];
}
