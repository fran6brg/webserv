#ifndef SERVER_HPP
#define SERVER_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <vector>
#include <sys/types.h> // pour les familles de protocoles (AF_INET, etc.)
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <netinet/ip.h>
#include <fcntl.h> // fcntl - Manipuler un descripteur de fichier http://manpagesfr.free.fr/man/man2/fcntl.2.html 
#include <sys/select.h> // select, fd_set

/*
** Headers
*/

#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

/*
** Class
*/

class Server
{
    /*
    ** member variables
    */

    private:

    protected:
        //

    public:
        std::string                 _name;
        int                         _port;
        int                         _socket_fd;
        struct sockaddr_in		    _addr;

        std::vector<Client,
        std::allocator<Client> >    _clients;        

    /*
    ** methods
    */

    private:
        Server();

    protected:
        //

    public:
        Server(std::string serverName, int port);
		// ~Server();
    
        int start(void);
        int connectClient(void);
        int recvRequest(std::vector<Client>::iterator it_c);
        int sendResponse(std::vector<Client>::iterator it_c);
        int handleClientRequest(std::vector<Client>::iterator it_c);

    /*
    ** friends
    */

    friend class Conf;
};

#endif