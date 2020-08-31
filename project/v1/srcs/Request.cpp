#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Request::Request(void)
{
    init();
}

/*
** other class methods
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

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

int Request::parse_request_line()
{
    std::string buf(_buffer, 1000);
    std::string line;
    ft_getline(buf, line);
    std::vector<std::string> tokens = split(line, ' ');

    std::cout << "tokens[1] = " << tokens[1] << std::endl;
    return (1);
}

int Request::parse()
{
    parse_request_line();

    return (1);
}