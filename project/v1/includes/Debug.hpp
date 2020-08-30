#ifndef DEBUG_HPP
#define DEBUG_HPP

/*
** Includes
*/

#include <sys/select.h> // fd_set

/*
** Class
*/

class Debug
{
    /*
    ** member variables
    */

    private:
        //

    protected:
        //

    public:
        //

    /*
    ** methods
    */

    private:
        //

    protected:
        //

    public:
        Debug();
		~Debug();
    
        void display_set(fd_set set) const;

    /*
    ** friends
    */

    friend class Conf;
};

#endif