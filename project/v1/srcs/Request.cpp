#include "../includes/Headers.hpp"
/*
** constructors / destructors
*/

Request::Request(void)
{
    init();
}

/*
** utils
*/

void	ft_getline(std::string &b, std::string &line)
{
    size_t					pos;

    pos = b.find("\n");

    if (pos != std::string::npos)
    {
        line = std::string (b, 0, pos++);
        b = b.substr(pos);
    }
    else
    {
        if (b[b.size() - 1] == '\n')
            b = b.substr(b.size());
        else
        {
            line = b;
            b = b.substr(b.size());
        }
    }
}

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

void print_map(std::stringstream &ss1, std::map<int, std::string> map)
{
    size_t i = 0;
    
    if (map.empty())
        ss1 << std::endl;
    else
    {
        while (i < map.size())
            ss1 << " " << map[i++];
        ss1 << std::endl;
    }
}

/*
** private class methods
*/

void Request::fill_request(std::string key, std::string value)
{
    // std::cout << "key: " << key << ", value: " << value << std::endl;

    size_t i = 0;
    std::vector<std::string> tokens;

    if (key == "Accept-Charset") // 4 example: Accept-Charset: utf-8, iso-8859-1;q=0.5
    {
        tokens = split(value, ',');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _accept_charset[i] = tokens[i];
                i++;
            }
        }
    }
    else if (key == "Accept-Language") // 5 example: Accept-Language: fr-CH, fr;q=0.9, en;q=0.8, de;q=0.7, *;q=0.5
    {
        tokens = split(value, ',');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _accept_language[i] = tokens[i];
                i++;
            }
        }
    }
    else if (key == "Authorization") // 6
        _authorization = value;
    else if (key == "Content-Language") // 7 example: Content-Language: de-DE || Content-Language: en-US || Content-Language: de-DE, en-CA
    {
        tokens = split(value, ',');
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
    else if (key == "Content-Location") // 9 example: Content-Location: <url>
        _content_location = value;
    else if (key == "Content-Type") // 10 example: Content-Type: text/html; charset=utf-8 || Content-Type: multipart/form-data; boundary=something
    {
        tokens = split(value, ';');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _content_type[i] = tokens[i];
                i++;
            }
        }
    }  
    else if (key == "Date") // 11
        _date = value;
    else if (key == "Host") // 12
        _host = value;
    else if (key == "Referer") // 13
        _referer = value;
    else if (key == "User-Agent") // 14
        _user_agent = value;

    // other headers not to be handled cf. subject
    else if (key == "Connection")
        ;
    else if (key == "Accept-Encoding")
        ;
    else if (key == "Accept")
        ;
}

/*
** public class methods
*/

void Request::init(void)
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
    _body.clear();

    // autres variables qui ne sont pas des headers
    _body_length = -1;
}

std::map<std::string, std::string> Request::headers_to_map(void)
{
    std::map<std::string, std::string> ret;

    /*
    ** Request Headers, dans l'ordre du sujet
    */ 

   ret["ACCEPT-CHARSET"] = map_to_string(_accept_charset, ';');
   ret["ACCEPT-LANGUAGE"] = map_to_string(_accept_language, ',');
   ret["AUTHORIZATION"] = _authorization;
   ret["CONTENT-LANGUAGE"] = map_to_string(_content_language, ',');
   ret["CONTENT-LENGTH"] = std::to_string(_content_length);
   ret["CONTENT-LOCATION"] = _content_location;
   ret["CONTENT-TYPE"] = map_to_string(_content_type, ';');
   ret["DATE"] = _date;
   ret["HOST"] = _host;
   ret["REFERER"] = _referer;
   ret["USER-AGENT"] = _user_agent;

   return (ret);
}

int Request::parse_request_line()
{
    std::string line;
    ft_getline(_buffer, line);
    std::vector<std::string> tokens = split(line, ' ');

    if (tokens.size() != 3)
        return (0);
    else
    {
        _method = tokens[0];
        _uri = tokens[1];
        _http_version = tokens[2];
    }
    return (1);
}

int Request::parse_headers()
{
    std::string line;
    std::size_t pos;
    std::string key;
    std::string value;
    
    // headers
    while (!_buffer.empty())
    {
        ft_getline(_buffer, line);
        // parse key/value before/after first ':'
        pos = line.find(":");
        if (pos == std::string::npos)
            break ;
        key = trim(line.substr(0, pos));
        value = trim(line.substr(pos + 1));
        if (key.empty())
            break ; 
        // fill corresponding request member variable with value
        fill_request(key, value);
    }
    return (1);
}

int Request::parse_body()
{

    if (_content_length) // meaning if value > 0 <=> a body exists
    {
        std::string line;
        std::vector<std::string> tokens;
        std::size_t pos;
        size_t i = 0;
        std::string key;
        std::string value;

        line.clear();
        ft_getline(_buffer, line);
        _body_length = line.length();
        line = line.substr(0, _content_length);
        tokens = split(line, '&');
        while (i < tokens.size())
        {
            line = tokens[i];
            pos = line.find("=");
            if (pos == std::string::npos) // format is not 'key=value'
            {
                _body[i++] = std::make_pair("", line);
            }
            else
            {
                key = trim(line.substr(0, pos));
                value = trim(line.substr(pos + 1));
                _body[i++] = std::make_pair(key, value);
            }
        }
    }
    return (1);
}

/*
**	Recuperer la location presente dans le fichier de config
**	a partir de l'uri present dans la requete
*/

int		Request::get_location(std::string *uri, std::vector<Location*> locations)
{
	int			j;
	std::string uri_tmp;
	// Verifie si l'uri est present dans les locations
	for (std::size_t i = 0; i < locations.size(); ++i)
	{
		if (locations[i]->_uri == *uri)
		{
			_location = locations[i];
			return (1);
		}
	}
	// Sinon check les sous dossiers de l'uri (jusqu'a "/") pour voir s'ils correspondent a une location 
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

/*
**	Definir le Path du fichier a traiter dans la reponse
**	a partir de l'uri present dans la requete, et de la location
*/

int	Request::parse_filename(std::vector<Location*> locations)
{
	struct stat	info;
	int			i;
	
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
				 if (S_ISDIR(info.st_mode)) // Verifier si le path est un dossier, si oui ajouter l'index (índex.html) a la fin du path
				 {
					i = _file.size() - 1;
					if (_file[i] == '/')
						_file = _file + _location->_index;
					else
						_file = _file + "/" + _location->_index;
				 }
			}
	}
	return (1);
}

int Request::parse(std::vector<Location*> location)
{
    parse_request_line();
	parse_filename(location);
    parse_headers();
    parse_body();
    return (1);
}

/*
** Debug
*/

void Request::display(void)
{
    size_t i;
    std::stringstream ss1;
    
	ss1 << "PARSED REQUEST:" << std::endl;

    ss1 << " 1) _method: " << _method << std::endl; // 1
    ss1 << " 2) _uri: " << _uri << std::endl; // 2
    ss1 << " 3) _http_version: " << _http_version << std::endl; // 3
    ss1 << " 4) _accept_charset:"; // 4
    print_map(ss1, _accept_charset);
	
     ss1 << " 5) _accept_language:"; // 5
    print_map(ss1, _accept_language);
    ss1 << " 6) _authorization: " << _authorization << std::endl; // 6
    ss1 << " 7) _content_language:"; // 7
    print_map(ss1, _content_language);
    ss1 << " 8) _content_length: " << _content_length << std::endl; // 8
    ss1 << " 9) _content_location:" << _content_location << std::endl; // 9
    ss1 << "10) _content_type:"; // 10
    print_map(ss1, _content_type);
    ss1 << "11) _date: " << _date << std::endl; // 11
    ss1 << "12) _host: " << _host << std::endl; // 12
    ss1 << "13) _referer: " << _referer << std::endl; // 13
    ss1 << "14) _user_agent: " << _user_agent << std::endl; // 14
    ss1 << "15) _body:" << std::endl; // 15
    if (_body.empty())
        ss1 << std::endl;
    else
    {
        i = 0;
        while (i < _body.size())
        {
            ss1 << " " << _body[i].first << "=" << _body[i].second;
            i++;
        }
        ss1 << std::endl;
    }
    ss1 << "16) _file: " << _file << std::endl;
	LOG_WRT(Logger::INFO, ss1.str());
}
