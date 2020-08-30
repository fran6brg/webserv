#ifndef PARSER_HPP
#define PARSER_HPP

/*
** Libraries
*/

#include <string>
#include <iostream>

/*
** Headers
*/

/*
** Class
*/

class Parser
{
    /*
    ** member variables
    */

    private:
        // 

    protected:
        //

    public:
        int     _accept_fd;

    /*
    ** methods
    */

    private:
        Parser();

    protected:
        //

    public:
        Parser(int accept_fd);


    /*
    ** friends
    */

    friend class Conf;
    friend class Server;
};

#endif