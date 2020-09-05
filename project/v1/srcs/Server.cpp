#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Server::Server(std::string serverName, int port):
    _name(serverName), _port(port), _socket_fd(-1)
{
    bzero(&_addr, sizeof(_addr));

	// --- > location et error a definir dans le prasing de la conf
	_error = "./www/error";
	
    Location *location1 = new Location("/", "./www", "index.html", "GET,POST,HEAD,OPTIONS");
	_locations.push_back(location1);

	Location *location2 = new Location("/test", "./www/test", "index.html", "DELETE");
	_locations.push_back(location2);
}

Server::~Server()
{
	std::cout << "server killed" << std::endl;
}

/*
** other class methods
*/

int Server::start(void)
{
    errno = 0;
	
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
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1) // cf. subject
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
    int ret = 0;

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

        Client *c = new Client(ret);
        _clients.push_back(c);

        return (1);
    }
}

int Server::recvRequest(Client *c)
{
    int ret = 0;
    errno = 0;
    
    // https://stackoverflow.com/questions/13736064/recv-connection-reset-by-peer
    // 'Connection reset by peer' has a number of causes,
    // but the most common one is that you have written to a connection that has already been closed by the peer.
    // In other words, an application protocol error.

    // https://stackoverflow.com/questions/24916937/how-to-catch-a-connection-reset-by-peer-error-in-c-socket
    // If you use non-blocking sockets, you also have to handle EWOULDBLOCK / EAGAIN here

    // https://stackoverflow.com/questions/49049430/difference-between-eagain-or-ewouldblock
    // - EWOULDBLOCK was defined for "operation would block" - that is, the operation would have blocked, but the descriptor was placed in non-blocking mode
    // - EAGAIN originally indicated when a "temporary resource shortage made an operation impossible"
        // Because the resource shortage was expected to be temporary, a subsequent attempt to perform the action might succeed (hence the name "again").
    
    if ((ret = recv(c->_accept_fd, c->_buffer, sizeof(c->_buffer), 0)) < 0)
    {
        std::cout << "error (recv - 1) " << _name << "/handleClientRequest/recv: " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else if (ret == 0)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            std::cout << "error (EWOULDBLOCK || EAGAIN) " << _name << "/handleClientRequest/recv: " << std::string(strerror(errno)) << std::endl;
        else
        {
            std::cout << _name << "(" << _port << ")" << " connection has been closed by the client (no error: " << std::string(strerror(errno)) << ")" << std::endl;
            c->_is_connected = false;
        }
        return (0);
    }
    else
        std::cout << _name << "(" << _port << ")" << ": recv() is ok" << std::endl;

    printf("\n\n****** request *******\n%s\n**********************\n\n", c->_buffer);

    c->_request._buffer = std::string(c->_buffer, 1000);
    c->_request.parse(_locations);
    c->_request.display();    
    return (1);
}

int Server::sendResponse(Client *c)
{
    int ret = 0;
    errno = 0;

	c->_response.handle_response(&(c->_request));
    c->_response.format_to_send(&(c->_request));
    
    if ((ret = send(c->_accept_fd, c->_response._to_send.c_str(), c->_response._to_send.size(), 0)) < 0)
    {
        std::cout << "error " << _name << "/handleClientRequest/send: " << std::string(strerror(errno)) << std::endl;
        return (0);
    }
    else
    {
        std::cout << _name << "(" << _port << ")" << ": send() is ok" << std::endl;
        c->_is_connected = false;
    }

    return (1);
}

int Server::handleClientRequest(Client *c)
{
    printf("handling request of client %i\n", c->_accept_fd);

    if (!recvRequest(c))
        return (0);
        
    if (!sendResponse(c))
        return (0);

    return (1);
}
