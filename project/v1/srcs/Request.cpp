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

void	ft_getline(std::string &buffer, std::string &line)
{
    size_t					pos;

    pos = buffer.find("\n");
    if (pos != std::string::npos)
    {
        line = std::string (buffer, 0, pos++);
        buffer = buffer.substr(pos);
    }
    else
    {
        if (buffer[buffer.size() - 1] == '\n')
            buffer = buffer.substr(buffer.size());
        else
        {
            line = buffer;
            buffer = buffer.substr(buffer.size());
        }
    }
}

std::vector<std::string> split(const std::string& str, char delim)
{
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return (str.substr(first, (last - first + 1)));
}

// void trim(std::string& str)
// {
//     size_t first = str.find_first_not_of(' ');
//     if (std::string::npos == first)
//         ;
//     size_t last = str.find_last_not_of(' ');
//     return (str.substr(first, (last - first + 1)));
// }

/*
** private class methods
*/

void Request::fill_request(std::string key, std::string value)
{
    std::cout << "key: " << key << ", value: " << value << std::endl;

    if (key == "Host")
        _host = value;
    else if (key == "Connection")
        ;
    else if (key == "Accept-Encoding")
        ;
    else if (key == "Accept")
        ;
    else if (key == "User-Agent")
        _user_agent = value;
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
    }
    return (1);
}

int Request::parse_headers()
{
    // std::cout << "PARSE:" << std::endl;
    std::string line;
    std::size_t pos;
    std::string key;
    std::string value;
    
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
        // fill corresponding request member variable
        fill_request(key, value);
        // parse la value selon la key
        // todo
    }
    return (1);
}

int Request::parse()
{
    parse_request_line();
    parse_headers();

    return (1);
}

void Request::display(void)
{
    std::cout << "PARSED REQUEST:" << std::endl;
    std::cout << "_method: " << _method << std::endl;
    std::cout << "_uri: " << _uri << std::endl;
    std::cout << "_http_version: " << _http_version << std::endl;
    // std::cout << "_accept_charset: " << _accept_charset << std::endl;
    // std::cout << "_accept_language: " << _accept_language << std::endl;
    // std::cout << "_authorization: " << _authorization << std::endl;
    // std::cout << "_content_language: " << _content_language << std::endl;
    std::cout << "_content_length: " << _content_length << std::endl;
    // std::cout << "_content_location: " << _content_location << std::endl;
    // std::cout << "_content_type: " << _content_type << std::endl;
    std::cout << "_date: " << _date << std::endl;
    std::cout << "_host: " << _host << std::endl;
    std::cout << "_referer: " << _referer << std::endl;
    std::cout << "_user_agent: " << _user_agent << std::endl;
}
