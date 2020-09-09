#ifndef CLIENT_HPP
#define CLIENT_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <sys/socket.h> // inet_ntoa()
#include <netinet/in.h> // inet_ntoa()
#include <arpa/inet.h> // inet_ntoa()

/*
** Headers
*/

#include "Request.hpp"
#include "Response.hpp"
class Server;

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
        Server     *_server;
        int         _accept_fd;
        std::string	_ip;
        int         _port;

        char        _buffer[1000]; // todo: quelle taille ? taille max des headers pour récup à minima le content_length
        Request     _request;
        Response    _response;

        bool        _is_connected;

    /*
    ** methods
    */

    private:
        Client();

    protected:
        //

    public:
        Client(Server *server, int accept_fd, struct sockaddr_in addr);
        ~Client();

        int parse_request(void);

    /*
    ** friends
    */

    friend class Server;
    friend class Request;
    friend class Response;
};

#endif