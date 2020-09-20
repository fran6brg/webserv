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
// #include "Request.hpp"
#include "Location.hpp"

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
		std::string					_host; // Add but never use
        std::string                 _name;
        int                         _port;
		std::string					_error;
        int                         _socket_fd;
        struct sockaddr_in		    _addr;

        std::vector<Location*>      _locations;        
        std::vector<Client*>        _clients;        

    /*
    ** methods
    */

    private:
        Server();

    protected:
        //

    public:
        Server(std::string serverName, int port, std::string host, std::string error_page);
		~Server();
    
        int start(void);
        int acceptNewClient(void);
        int recvRequest(Client *c);
        int sendResponse(Client *c);
        int handleClientRequest(Client *c);

    /*
    ** friends
    */

    friend class Conf;
};


#endif
