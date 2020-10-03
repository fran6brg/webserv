#ifndef CONF_HPP
#define CONF_HPP

/*
** Const
*/

#define CLIENT_CONNECTION_TIMEOUT  60
#define OPEN_MAX_PADDING           10

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>	
#include <signal.h>	
#include <list>

/*
** Headers
*/

#include "Server.hpp"
#include "Config_parser.hpp"

class Server;

/*
** Class
*/

class Conf
{
    /*
    ** member variables
    */

    private:
        //

    protected:
        //

    public:
        bool                    _on;
        std::string             _webserv;

        // select() related
        int                     _nfds;
        fd_set					_readfds;
        fd_set					_save_readfds;
        fd_set					_writefds;
        fd_set					_save_writefds;
        fd_set					_exceptfds;
        fd_set					_save_exceptfds;
        struct timeval			_timeout;
        
        // other variables
        std::vector<Server*>	_servers;
        std::list<int>          _active_fds;
        
        // debug
        int                     _nb_accepted_connections;
    
    /*
    ** methods
    */

    private:
        void loop_fd_set() const;

    protected:
        //

    public:
        Conf();

        int parse(char *conf);
        void reset_fd_sets(void);
        int get_nfds(void) const;
        int get_nb_open_fds(void) const;

        void add_fd(int fd);
        void remove_fd(int fd);
        
        int run_select();
};

#endif
