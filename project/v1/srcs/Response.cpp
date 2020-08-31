#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Response::Response(void)
{
    init();
}

/*
** other class methods
*/

void Response::init(void)
{
    _http_version.clear();
}