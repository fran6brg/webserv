#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Server::Server(std::string serverName, int port):
    _name(serverName), _port(port), _socket_fd(-1)
{
    bzero(&_addr, sizeof(_addr));
}

/*
** other class methods
*/

int Server::start(void)
{
	// socket
	if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
        std::cout << "error " << _name << "/start/socket(): " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": socket_fd created (" << _socket_fd << ")" << std::endl;

	
	// SO_REUSEADDR option on the listening socket: to avoid “Address already in use” error when binding(). 
    // How to use setsockopt() with the SO_REUSEADDR option?
    // https://stackoverflow.com/questions/21515946/what-is-sol-socket-used-for
    int value = 1; // This will set the SO_REUSEADDR in my socket to 1.
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
	{
        std::cout << "error " << _name << "/start/setsockopt(): " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": setsockopt() is ok" << std::endl;

	// set struct sockaddr_in
	_addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(INADDR_ANY); // https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
    _addr.sin_port = htons(_port);

	if (bind(_socket_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
        std::cout << "error " << _name << "/start/bind(): " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": bind() is ok" << std::endl;

    if (listen(_socket_fd, 10) == -1)
	{
        std::cout << "error " << _name << "/start/listen(): " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": listen() is ok" << std::endl;

	// http://beej.us/guide/bgnet/html/#selectman
	// Note for Linux users: Linux’s select() can return “ready-to-read” and then not actually be ready to read, thus causing the subsequent read() call to block.
	// You can work around this bug by setting O_NONBLOCK flag on the receiving socket so it errors with EWOULDBLOCK, then ignoring this error if it occurs. 
	// See the fcntl() reference page for more info on setting a socket to non-blocking.
	// fcntl - Manipuler un descripteur de fichier
	// O_NONBLOCK ou O_NDELAY : Le fichier est ouvert en mode « non bloquant ».
	// Ni la fonction open() ni aucune autre opération ultérieure sur ce fichier ne laissera le processus appelant en attente.
	// Pour la manipulation des FIFO (tubes nommés), voir également fifo(7). Pour une discussion sur l'effet de O_NONBLOCK conjointement aux verrouillages de fichier impératifs et aux baux de fichiers, voir fcntl(2).
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
	{
        std::cout << "error " << _name << "/start/fcntl: " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": fcntl() is ok" << std::endl;
	
    // important: on ajoute _socket_fd à la liste des fd à surveiller pour recevoir une requête
	FD_SET(_socket_fd, &g_conf._save_readfds);    
    return (1);
}


int Server::connectClient(void)
{
    int ret = -1;

    if ((ret = accept(_socket_fd, NULL, NULL)) == -1)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  accept() failed");
            g_conf._on = false;
        }
        else
            std::cout << "error " << _name << "/connectClient/accept: " << std::string(strerror(errno)) << std::endl;
        return (0);

    }
    else
    {
        std::cout << _name << "(" << _port << ")" << ": accepted client on fd " << ret << std::endl;
        
        Client c = Client(ret);
        _clients.push_back(c);

        return (1);
    }
}

int Server::handleClientRequest(std::vector<Client>::iterator it_c)
{
    printf("handling request of client %i\n", it_c->_accept_fd);

    char buffer[1000];
    int rc = 1;

    if ((rc = recv(it_c->_accept_fd, buffer, sizeof(buffer), 0)) < 0)
    {
        std::cout << "error " << _name << "/handleClientRequest/recv: " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": recv() is ok" << std::endl;

    printf("\n\n************ request *************\n%s\n**********************************\n\n", buffer);

    char hello[108] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 44\n\n<html><body><h1>It works!</h1></body></html>";

    if ((rc = send(it_c->_accept_fd, hello, sizeof(hello), 0)) < 0)
    {
        std::cout << "error " << _name << "/handleClientRequest/send: " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": send() is ok" << std::endl;

    return (1);
}