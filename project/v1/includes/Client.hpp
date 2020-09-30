#ifndef CLIENT_HPP
#define CLIENT_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
** Headers
*/

#include "Request.hpp"
#include "Response.hpp"
class Server;

/*
** Const
*/

# define RECV_BUFFER    65536

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
        Server      *_server;
        int         _accept_fd;
        std::string	_ip;
        int         _port;

        Request     _request;
        Response    _response;

        bool        _is_finished;
        bool        _is_connected;

        char        *_buffermalloc;
		std::string	_concat_body;
        int         _line_size;

		std::string	_last_active_time;
        int         _wfd;
        int         _rfd;
        int 	    _pid;
        int         _read_ok;

    	enum status
    	{
			HEADER,
			BODY,
			COMPLETE,
			WAITING,
			ERROR
    	};
        
		int	        recv_status;

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

        void reset(void);
        void	write_file();
        void	read_file(std::string &buff);

    /*
    ** friends
    */

    friend class Server;
    friend class Request;
    friend class Response;
};

#endif
