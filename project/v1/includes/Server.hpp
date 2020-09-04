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

std::vector<std::string> split(const std::string& str, char delim);

/*
** Class
*/

class Server
{
    /*
    ** class
    */

    public:
        class Location 
        {
            private:
            Location();	 		
            public:
            Location(std::string uri, std::string root, std::string index, std::string method)
            {
                _uri = uri;
                _root = root;
                _index = index;
                _method = split(method, ',');
            };
            
            std::string 				_uri;
            std::string 				_root;
            std::string					_index;
            std::vector<std::string>	_method;
        };

    /*
    ** member variables
    */

    private:

    protected:
        //

    public:
        std::string                 _name;
        int                         _port;
		std::string					_error;
        int                         _socket_fd;
        struct sockaddr_in		    _addr;

		std::string					_root;
		std::string					_index;

        std::vector<Location*>        _location;        

        std::vector<Client*>        _clients;        

    /*
    ** methods
    */

    private:
        Server();

    protected:
        //

    public:
        Server(std::string serverName, int port);
		~Server();
    
        int start(void);
        int connectClient(void);
        int recvRequest(Client *c);
        int sendResponse(Client *c);
        int handleClientRequest(Client *c);

    /*
    ** friends
    */

    friend class Conf;
};


#endif
