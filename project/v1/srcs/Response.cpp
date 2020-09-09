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
    _content_length = -1; // est-ce qu'on peut avoir un content_length de 0 ?
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
    if (_content_length != -1)
        ss << "Content-Length: " << _content_length << "\r\n";
    // if (!_content_location.empty())     { ss << "content_location: " << _content_location << "\r\n"; }
    if (!_allow.empty())     { ss << "allow: " << _allow << "\r\n"; }
    if (!_content_type.empty())
	{
		ss << "Content-Type:";
    	while (i < _content_type.size())
    	{
    	    ss << " " << _content_type[i++];
    	    if (i + 1 < _content_type.size())
    	        ss << ";";
    	}
    	ss << "\r\n";
	}
    if (!_last_modified.empty())        { ss << "Last-Modified: " << _last_modified << "\r\n"; }
    if (!_location.empty() && _status_code == 201)             { ss << "Location: " << _location << "\r\n"; }
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
    _content_length = _body.size();
	
	// Response headers, dans l'ordre du sujet
    _date = get_date();
    _server = g_conf._webserv;
	if ((req->_method == "GET" || req->_method == "HEAD" || req->_method == "PUT" || req->_method == "POST")
    && (_status_code == OK_200 || _status_code == CREATED_201))
		 _content_type[1] = get_content_type(req->_file);
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
}

void			Response::get(Request *req)
{
	std::ifstream file(req->_file);
	if (file.good())
	{
		std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_body = buffer;
		_last_modified = get_last_modif(req->_file);
		_status_code = OK_200;
	}
	// else
	// 	bad_request(req);
}

void			Response::post(Request *req)
{
	(void)req;
    // set env vars
    create_env_tab(req);
    // create args
    // fork
    // pipe
    // launch execve
}

void			Response::put(Request *req)
{

    if (req->_content_length == -1)
    {
	    // std::cout << "inside put: BAD_REQUEST_400" << std::endl;
        _status_code = BAD_REQUEST_400;
        return ;
    }

    // if (req->_content_length != req->_body_length)
    // {
	//     std::cout << "inside put: REQUEST_ENTITY_TOO_LARGE_413: " << req->_content_length << " vs " <<  req->_body_length << std::endl;
    //     _status_code = REQUEST_ENTITY_TOO_LARGE_413;
    //     return ;
    // }

	std::ifstream f1(req->_file);
	if (f1.good()) // test if _file already exists
	{
        _status_code = OK_200;
    }
	else // _file does not exist
	{
        _status_code = CREATED_201;
    }
	f1.close();

	std::ofstream f2(req->_file);
	if (f2.good())
	{
	    // std::cout << "inside put: writing body in _file" << std::endl;
        f2 << req->_body[0].second.substr(0, req->_content_length) << std::endl;
    }
	else
	{
	    std::cout << "inside put: failed to write body in _file" << std::endl;
    }
	f2.close();
}

void			Response::ft_delete(Request *req)
{
	int	ret;
	std::ifstream file(req->_file);
	if (file.good())
	{
		ret = remove(req->_file.c_str()); // A remplacer par unlink() ? pourquoi ? A voir
		if (!ret)
			_status_code = OK_200;
		else
			_status_code = ACCEPTED_202; // requete accepte mais la supression a echoue (pas sur d'en avoir besoin)
	}
	else
		_status_code = NO_CONTENT_204;
}

void			Response::option(Request *req)
{
	std::string buffer;

	for (std::size_t i = 0; i < (req->_location->_method).size(); ++i)
	{
		buffer = buffer + (req->_location->_method)[i];
		if (i != (req->_location->_method).size() - 1)
			buffer = buffer + ", ";
	}
	_status_code = OK_200;
	_allow = buffer;
}

int				Response::method_not_allowed(Request *req)
{
	// Comparaison de la methode demande avec les methodes autorise dans la location
	for (std::size_t i = 0; i < (req->_location->_method).size(); ++i)
	{
		if ((req->_location->_method)[i] == req->_method)
			return (0);
	}
    _status_code = METHOD_NOT_ALLOWED_405;
	std::string path = "./www/error/405.html";
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
         || req->_method == "OPTIONS");
}

int				Response::bad_request(Request *req)
{
	if (accepted_method(req))
        return (0);
    _status_code = BAD_REQUEST_400;
	std::string path = "./www/error/400.html";
    std::ifstream error400(path);
    std::string buffer((std::istreambuf_iterator<char>(error400)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}
