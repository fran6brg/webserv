#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Client::Client(int accept_fd):
    _accept_fd(accept_fd)
{
    ;
}

/*
** other class methods
*/

int Client::parse_request(void)
{
    _request.init();
    // _request.parse_request_line(this);
    // _request.parse_headers(this);
    // _request.parse_body(this);
    return (1);
}