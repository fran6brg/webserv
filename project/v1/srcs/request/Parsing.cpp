#include "../../includes/Headers.hpp"

/*
** 1. Request line
*/

int     Request::parse_request_line()
{
    std::string line;
    std::vector<std::string> tokens;

    while (1)
    {
        line.clear();
        utils_tmp::ft_getline(_buffer, line);
        tokens = utils_tmp::split(line, ' ');
        if (tokens.size() == 3)
            break ;
    }    
    LOG_WRT(Logger::DEBUG, "Request::parse_request_line() = " + line);

    _method = tokens[0];
    _uri = tokens[1];
    _http_version = tokens[2];
    return (1);
}

/*
** 2. Headers
*/

int     Request::parse_headers()
{
    std::string line;
    std::size_t pos;
    std::string key;
    std::string value;
    
    // headers
    while (!_buffer.empty())
    {
        utils_tmp::ft_getline(_buffer, line);
        // parse key/value before/after first ':'
        pos = line.find(":");
        if (pos == std::string::npos)
            break ;
        key = utils_tmp::trim(line.substr(0, pos));
        utils_tmp::remove_return(key);
        value = utils_tmp::trim(line.substr(pos + 1));
        utils_tmp::remove_return(value);
        if (key.empty())
            break ; 
        // fill corresponding request member variable with value
        fill_request(key, value);
    }
    return (1);
}

void    Request::fill_request(std::string key, std::string value)
{
    size_t i = 0;
    std::vector<std::string> tokens;

    if (key == "Accept-Charset")
    {
        tokens = utils_tmp::split(value, ',');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _accept_charset[i] = tokens[i];
                i++;
            }
        }
    }
    else if (key == "Accept-Language")
    {
        tokens = utils_tmp::split(value, ',');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _accept_language[i] = tokens[i];
                i++;
            }
        }
    }
    else if (key == "Authorization")
        _authorization = value;
    else if (key == "Content-Language")
    {
        tokens = utils_tmp::split(value, ',');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _content_language[i] = tokens[i];
                i++;
            }
        }
    }        
    else if (key == "Content-Length") // 8
        _content_length = std::stoi(value);
    else if (key == "Content-Location") // 9
        _content_location = value;
    else if (key == "Content-Type") // 10
        _content_type = value;
    else if (key == "Date") // 11
        _date = value;
    else if (key == "Host") // 12
        _host = value;
    else if (key == "Referer") // 13
        _referer = value;
    else if (key == "User-Agent") // 14
        _user_agent = value;
    else if (key == "Transfer-Encoding") // 17
        _transfer_encoding = value;
    else if (key == "X-Secret-Header-For-Test") // ?
        _secret_header = value;
    else if (key == "Keep-Alive") // ?
        _keep_alive = value;
    else if (key == "Connection")
        ;
    else if (key == "Accept-Encoding")
        ;
    else if (key == "Accept")
        ;
}

/*
** 3. Filename
*/

void    Request::create_autoindex()
{
    /*
    **	Definir le Path du fichier a traiter dans la reponse
    **	a partir de l'uri present dans la requete, et de la location
    */

	DIR				*dir;
	struct dirent	*dent;

	dir = opendir(_file.c_str());
	if (dir)
	{
		_client->_response._body += "<html><body><h1>";
		while ((dent = readdir(dir)) != NULL)
		{
			if (std::string(dent->d_name) != "..")
			{
				_client->_response._body += dent->d_name;
				_client->_response._body += "\n";
			}
		}
		closedir(dir);
		_client->_response._body += "</h1></body></html>";
	}
	std::cout << _client->_response._body;
}

int	    Request::get_location(std::string *uri, std::vector<Location*> locations)
{
    /*
    **	Recuperer la location presente dans le fichier de config
    **	a partir de l'uri present dans la requete
    */
	int			j;
	std::string uri_tmp;

	for (std::size_t i = 0; i < locations.size(); ++i)
	{
		if (locations[i]->_uri == *uri)
		{
			_location = locations[i];
			return (1);
		}
	}
	j = (*uri).size() - 1;
	uri_tmp = *uri;
    while (uri_tmp.size() > 0)
	{
		while (uri_tmp[j] != '/' && j != 0)
			j--;
		uri_tmp = uri_tmp.substr(0, j);
		if (uri_tmp == "")
			uri_tmp = "/";
		for (std::size_t i = 0; i < locations.size(); ++i)
		{
			if (locations[i]->_uri == uri_tmp)
			{
				_file = (*uri).substr(j + 1, (*uri).size());
				_file_name = (*uri).substr(j + 1, (*uri).size());
				_location = locations[i];
				return (1);
			}
		}
	}
	return (0);
}

int	    Request::parse_filename(std::vector<Location*> locations)
{
	struct stat	info;
	int			i;


	parse_query_string();	
	get_location(&_uri, locations);

	if (_location)
	{
		i = (_location->_root).size() - 1;
		if ((_location->_root)[i] == '/')
			_file = _location->_root + _file;
		else
			_file = _location->_root + "/" + _file;
		if (stat(_file.c_str(), &info) == 0)
		{
			 if (S_ISDIR(info.st_mode))
			 {
				if (_location->_autoindex == 1 && _method == "GET")
					create_autoindex();
				else
				{
					i = _file.size() - 1;
					if (_file[i] == '/')
						_file = _file + _location->_index;
					else
						_file = _file + "/" + _location->_index;
			 	}
			 }
		}
	}
	return (1);
}

/*
** 1. Request line + 2. Headers + 3. Filename
*/

int     Request::parse(std::vector<Location*> location)
{
    LOG_WRT(Logger::DEBUG, "Request::parse()");
    
    parse_request_line();
	parse_filename(location);
    parse_headers();

	return (RET_SUCCESS);
}

/*
** ----------------------------------------------------------------------------
*/

/*
** Body as normal
*/

void    Request::parse_body_length()
{
	char		*buff = _client->_buffermalloc;
	std::string &body = _client->_request._text_body;
	size_t 		cut;

	_client->_concat_body.append(buff);
	if (_content_length < 0)
	{
		_client->recv_status = Client::ERROR;
		return ;
	}
	size_t new_body_size = body.length() + _client->_concat_body.length();
	LOG_WRT(Logger::DEBUG, " body.length()= "+std::to_string( body.length()) + " strlen(buff="+std::to_string(strlen(buff)));
	LOG_WRT(Logger::DEBUG, "new_body_size= "+std::to_string(new_body_size) + " _content_length="+std::to_string(_content_length));
	if (new_body_size >= _content_length)
	{
		cut = _content_length - body.length();
		body += _client->_concat_body.substr(0, cut);
        LOG_WRT(Logger::DEBUG, "Request::parse_body_length(): Client::COMPLETE");
		_client->recv_status = Client::COMPLETE;
		memset(buff, 0, RECV_BUFFER + 1);
	}
	else
	{
		body += _client->_concat_body;
		memset(buff, 0, RECV_BUFFER + 1);
	}
}

/*
** Body as chunked
*/

void    Request::parse_body_chunked()
{
    LOG_WRT(Logger::DEBUG, "start parse_body_chunked()");

	char				*recv_buffer = _client->_buffermalloc;
	std::string 		&body = _client->_request._text_body;
	size_t				pos;
	
    _client->_concat_body.append(recv_buffer);

    while (true)
	{
        LOG_WRT(Logger::DEBUG, "--- start while ---");
        LOG_WRT(Logger::DEBUG, "_client->_concat_body len = " + std::to_string(_client->_concat_body.length()));
        LOG_WRT(Logger::DEBUG, "_client->_line_size       = " + std::to_string(_client->_line_size));
        pos = _client->_concat_body.find("\r\n");
        if (pos == std::string::npos)
            break ;
        else
        {
            // 1. get _client->_line_size
            if (_client->_line_size == -1)
            {
                std::string line = _client->_concat_body.substr(0, pos);
                LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): line                = -" + line + "-");
                _client->_line_size = utils_tmp::hexa_to_dec(line.c_str());
                LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): _client->_line_size = " + std::to_string(_client->_line_size));
                _client->_concat_body.erase(0, line.length() + 2);
            }
            // 2. get next line content for _client->_line_size first bytes
            if (_client->_line_size > 0)
            {
                LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): _client->_concat_body.length() = " + std::to_string(_client->_concat_body.length()));
                if (_client->_concat_body.length() >= _client->_line_size)// todo: if hexa is bigger than predict ?
                {
                    pos = _client->_concat_body.find("\r\n");
                    LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): pos = " + std::to_string(pos));
                    if (pos == std::string::npos)
                        break ;
                    else if (_location->_max_body != 1 && pos > _location->_max_body)
                    {
                        _saved_error = REQUEST_ENTITY_TOO_LARGE_413;
                        LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): REQUEST_ENTITY_TOO_LARGE_413");
                    }
                    LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): substr(0, " + std::to_string(_client->_line_size) + ")");
                    body.append(_client->_concat_body.substr(0, _client->_line_size));
                    _client->_concat_body.erase(0, _client->_line_size + 2);
                    _client->_line_size = -1;
                }
                else
                {
                    LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): _client->_concat_body.length() < _client->_line_size");
                    break ;
                }
            }
            // 3. end chunked body parsing
            else if (_client->_line_size == 0)
            {
                LOG_WRT(Logger::DEBUG, "Request::parse_body_chunked(): Body fully parsed");
                _client->recv_status = Client::COMPLETE;
                break ;
            }
        }
        LOG_WRT(Logger::DEBUG, "--- end while ---\n");
	}
	memset(recv_buffer, 0, RECV_BUFFER + 1);
    LOG_WRT(Logger::DEBUG, "end parse_body_chunked()");
}

void    Request::update_body()
{
	if (_transfer_encoding == "chunked")
	{
        parse_body_chunked();
    }
	else if (_content_length >= 0)
	{
        parse_body_length();
    }
    else
    {
        LOG_WRT(Logger::DEBUG, "Request::update_body(): Client::COMPLETE");
		_client->recv_status = Client::COMPLETE;
    }    
}

/*
** ----------------------------------------------------------------------------
*/

/*
** Query string
*/

void	Request::parse_query_string()
{
	int				i = 0;
	std::string		uri = _uri;	
	
	while (uri[i] && uri[i] != '?')
		i++;
	if (uri[i] == '?')
	{
		_query = uri.substr(i + 1, uri.size());
		_uri = uri.substr(0, i);
	}
}
