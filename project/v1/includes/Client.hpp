#ifndef CLIENT_HPP
#define CLIENT_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>

/*
** Headers
*/

#include "Request.hpp"
#include "Response.hpp"

/*
** Class
*/

class Client
{
    /*
    ** member variables
    */

    private:
        // 

    protected:
        //

    public:
        int         _accept_fd;
        char        _buffer[1000];
        Request     _request;
        Response    _response;

    /*
    ** methods
    */

    private:
        Client();

    protected:
        //

    public:
        Client(int accept_fd);
        ~Client();

        int parse_request(void);

    /*
    ** friends
    */

    // friend class Conf;
    friend class Server;
};

#endif