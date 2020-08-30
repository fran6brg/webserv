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

int parse_request(void)
{
    _request.init();
}