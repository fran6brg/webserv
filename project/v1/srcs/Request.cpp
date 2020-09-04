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

    if (key == "Accept-Charset") // 4
    {
        tokens = split(value, ' ');
        if (!tokens.empty())
        {
            while (i < tokens.size())
            {
                _accept_charset[i] = tokens[i];
                i++;
            }
        }
    }
    else if (key == "Accept-Language") // 5
    {
        tokens = split(value, ' ');
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
    else if (key == "Content-Length") // 8
        _content_length = std::stoi(value);
    else if (key == "Host") // 12
        _host = value;
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
    _content_length = 0;
    _content_location.clear();
    _content_type.clear();
    _date.clear();
    _host.clear();
    _referer.clear();
    _user_agent.clear();
    // Request body
    _body.clear();
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
//		_file = "./www" + _uri;
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

        ft_getline(_buffer, line);
        line = line.substr(0, _content_length);
        // std::cout << "last line = " << line << std::endl;
        tokens = split(line, '&');
        while (i < tokens.size())
        {
            line = tokens[i];
            pos = line.find("=");
            if (pos == std::string::npos) // format is not key=value
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

int	Request::parse_filename(std::string root, std::string index)
{
	int		i;

	i = _uri.size();
	if (_uri[i - 1] == '/')
		_file = root + _uri + index;
	else
		_file = root + _uri;
	return (1);
}

int Request::parse(std::string root, std::string index)
{
    parse_request_line();
	parse_filename(root, index);
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
    ss1 << " 9) _content_location:"; // 9
    print_map(ss1, _content_location);
    ss1 << "10) _content_type:"; // 10
    print_map(ss1, _content_type);
    ss1 << "11) _date: " << _date << std::endl; // 11
    ss1 << "12) _host: " << _host << std::endl; // 12
    ss1 << "13) _referer: " << _referer << std::endl; // 13
    ss1 << "14) _user_agent: " << _user_agent << std::endl; // 14
    ss1 << "15) _body:" << std::endl; // 15
    ss1 << "16) _file (location du fichier de la requete ?): " << _file << std::endl;
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

    std::cout << ss1.str() << std::endl;
}
