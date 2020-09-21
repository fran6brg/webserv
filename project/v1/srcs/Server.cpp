#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Server::Server(std::string serverName, int port, std::string host, std::string error_page):
    _name(serverName), _port(port), _socket_fd(-1), _error(error_page), _host(host)
{
    bzero(&_addr, sizeof(_addr));

	// --- > location et error a definir dans le prasing de la conf
	//_error = "./www/old/error";
	
    //Location *location1 = new Location("/", "./www/old", "index.html", "GET,POST,HEAD,OPTIONS,TRACE");
	//_locations.push_back(location1);

	//Location *location2 = new Location("/test", "./www/old/test", "index.html", "DELETE");
	//_locations.push_back(location2);
}

Server::~Server()
{
	LOG_WRT(Logger::INFO, _name + "status killed");
    g_conf.remove_fd(_socket_fd); 
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
		LOG_WRT(Logger::ERROR, "Server::start -> socket(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> socket=" + std::to_string(_socket_fd));

	
	// SO_REUSEADDR option on the listening socket: to avoid “Address already in use” error when binding(). 
    // How to use setsockopt() with the SO_REUSEADDR option?
    // https://stackoverflow.com/questions/21515946/what-is-sol-socket-used-for
    int value = 1; // This will set the SO_REUSEADDR in my socket to 1.
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
	{
		LOG_WRT(Logger::ERROR, "Server::start -> setsockopt(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> setsockopt=OK");

	// set struct sockaddr_in
	_addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = htonl(INADDR_ANY); // https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
    _addr.sin_port = htons(_port);

	if (bind(_socket_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		LOG_WRT(Logger::ERROR, "Server::start -> bind(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> bind=OK");

    if (listen(_socket_fd, 42) == -1)
	{
		LOG_WRT(Logger::ERROR, "Server::start -> listen(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> listen=OK");

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
		LOG_WRT(Logger::ERROR, "Server::start -> fcntl(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> fcntl=OK");
	
    // important: on ajoute _socket_fd à la liste des fd à surveiller pour recevoir une requête
	FD_SET(_socket_fd, &g_conf._save_readfds);
    g_conf.add_fd(_socket_fd);    
    return (1);
}


int Server::acceptNewClient(void)
{
    int accept_fd = 0;
    struct sockaddr_in	client_addr;
    int addrlen = sizeof(client_addr);

    bzero(&client_addr, sizeof(client_addr));
    if ((accept_fd = accept(_socket_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
        {
			LOG_WRT(Logger::ERROR, "Server::acceptNewClient -> accept(): " + std::string(strerror(errno)));
            g_conf._on = false;
        }
        else
			LOG_WRT(Logger::ERROR, "Server::acceptNewClient -> accept(): " + std::string(strerror(errno)));
        return (0);
    }
    else
    {
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> accept_fd = " + std::to_string(accept_fd));
        Client *c = new Client(this, accept_fd, client_addr);
        _clients.push_back(c);
		LOG_WRT(Logger::INFO, _name + " has now " + std::to_string(_clients.size()) + " clients connected");

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
    
    // If we try to read from a non-blocking socket and there's no data there,
    // it's not allowed to block-it will return -1 and errno will be set to EWOULDBLOCK.
    // The non-blocking mode is set by changing one of the socket's flags.

    // bzero(c->_buffer, 1000);

    int bytes = strlen(c->_buffermalloc);
    ret = recv(c->_accept_fd, c->_buffermalloc + bytes, RECV_BUFFER - bytes, 0);
    bytes += ret;
    // if ((ret = recv(c->_accept_fd, c->_buffer, sizeof(c->_buffer), 0)) == -1)
	if (ret == -1)
    {
		LOG_WRT(Logger::ERROR, "Server::recvRequest -> recv(): " + std::string(strerror(errno)));
        // https://stackoverflow.com/questions/10318191/reading-socket-eagain-resource-temporarily-unavailable
        // EAGAIN does not mean you're disconnected, it just means "there's nothing to read now; try again later".
        if (errno != EWOULDBLOCK /*|| errno != EAGAIN*/)
            c->_is_connected = false;
        return (0);
    }
    else if (ret == 0)
    {
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> client(" + std::to_string(c->_accept_fd) + ") closed");
        c->_is_connected = false;
        return (0);
    }
    else
	{
        // c->_buffer[ret] = '\0';
        c->_buffermalloc[bytes] = '\0';
        if (strstr(c->_buffermalloc, "\r\n\r\n") != NULL) // meaning all headers are fully recv
        {
            LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> recv=OK");
            
            LOG_WRT(Logger::INFO, "RAW REQUEST (" + std::to_string(ret) +"):\n" + "--------------\n" + std::string(c->_buffermalloc, bytes) + "\n----------------------");
            
            // c->_request._buffer = std::string(c->_buffer, 1000);
            c->_request._buffer = std::string(c->_buffermalloc, bytes);
            c->_request.parse(_locations);

            // FD_CLR(c->_accept_fd, &g_conf._save_readfds); // working without but to test
            // FD_CLR(c->_accept_fd, &g_conf._readfds); // working without but to test

            FD_SET(c->_accept_fd, &g_conf._save_writefds);
            // FD_SET(c->_accept_fd, &g_conf._writefds); // working without but to test
            
            c->_request.display();
            return (1);
        }
        else
        {
            LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> recv=not complet");
            return (0);
        }
    }
}

int Server::sendResponse(Client *c)
{
    int ret = 0;
    errno = 0;

	c->_response.handle_response(&(c->_request));
    c->_response.format_to_send(&(c->_request));
    
    if ((ret = send(c->_accept_fd, c->_response._to_send.c_str(), c->_response._to_send.size(), 0)) == -1)
    {
        LOG_WRT(Logger::ERROR, "Server::sendResponse -> send(): " + std::string(strerror(errno)));
        // if (errno != EWOULDBLOCK && errno != EAGAIN)
            c->_is_connected = false;
        return (0);
    }
    else
    {
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> send=OK");
        // LOG_WRT(Logger::DEBUG, "sendResponse: date: " + c->_response._date);
        // LOG_WRT(Logger::DEBUG, "sendResponse: to_send: " + c->_response._to_send);
        c->_is_connected = false;
    }
    return (1);
}

int Server::handleClientRequest(Client *c)
{
    int ok_read = 0;
    int ok_write = 0;

    if (FD_ISSET(c->_accept_fd, &g_conf._readfds))
    {
        LOG_WRT(Logger::INFO, "reading request of client " + std::to_string(c->_accept_fd) + "\n");
        if (!recvRequest(c))
            return (0);
        else
            ok_read = 1;
    }
    else
        LOG_WRT(Logger::INFO, "reading not set for client " + std::to_string(c->_accept_fd) + "\n");

    if (FD_ISSET(c->_accept_fd, &g_conf._writefds))
    {
        LOG_WRT(Logger::INFO, "sending response to client " + std::to_string(c->_accept_fd) + "\n");
        if (!sendResponse(c))
            return (0);
        else
            ok_write = 1;
    }
    else
        LOG_WRT(Logger::INFO, "writing not set for client " + std::to_string(c->_accept_fd) + "\n");

    return (ok_read && ok_write);
}
