#ifndef CONF_HPP
#define CONF_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/select.h> // select, fd_set
#include <string.h>		// strerror linux

/*
** Headers
*/

#include "Server.hpp"

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
        // loop
        bool                    _on;
        std::string             _webserv;

        // select() related
        fd_set					_readfds;
        fd_set					_save_readfds;
        fd_set					_writefds;
        fd_set					_save_writefds;
        fd_set					_exceptfds;
        fd_set					_save_exceptfds;
        struct timeval			_timeout;
        // int                     _nfds;
        
        // other variables
        std::vector<Server*>	    _servers;

    /*
    ** methods
    */

    private:
        void loop_fd_set() const;

    protected:
        //

    public:
        Conf();
		// ~Conf();

        // int parse(char *filename);
        int parse(void);
        void reset_fd_sets(void);
        int get_nfds(void) const;
        int run_select();
};

#endif