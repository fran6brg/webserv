#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Server::Server(std::string serverName, int port, std::string host, std::string error_page):
    _name(serverName), _port(port), _socket_fd(-1), _error(error_page), _host(host)
{
    bzero(&_addr, sizeof(_addr));
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

    if (listen(_socket_fd, 242) == -1)
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
	if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		LOG_WRT(Logger::ERROR, "Server::start -> fcntl(): " + std::string(strerror(errno)));
        return (0);
    }
    else
		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> fcntl=OK");
	
	FD_SET(_socket_fd, &g_conf._save_readfds);
    g_conf.add_fd(_socket_fd);    

    LOG_WRT(Logger::INFO, "****************\n\n");
    return (1);
}

int Server::acceptNewClient(void)
{
    int accept_fd = 0;
    struct sockaddr_in	client_addr;
    int addrlen = sizeof(client_addr);

    LOG_WRT(Logger::INFO, std::string(MAGENTA_C) + "get_nb_open_fds() = " + std::to_string(g_conf.get_nb_open_fds()) + " | OPEN_MAX = " + std::to_string(OPEN_MAX) + std::string(RESET));

    bzero(&client_addr, sizeof(client_addr));
    if ((accept_fd = accept(_socket_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) == -1)
    {
        LOG_WRT(Logger::ERROR, "Server::acceptNewClient -> accept(): " + std::string(strerror(errno)));
        return (0);
    }
    else
    {
		LOG_WRT(Logger::DEBUG, _name + "(" + std::to_string(_port) + ") -> accept_fd = " + std::to_string(accept_fd));	
        Client	*c = new Client(this, accept_fd, client_addr);

        if (g_conf.get_nb_open_fds() > 400 - OPEN_MAX_PADDING)
        {
            LOG_WRT(Logger::INFO, std::string(MAGENTA_C)
                + "service unavailable (503) on "
                + _name
                + " get_nb_open_fds() = "
                + std::to_string(g_conf.get_nb_open_fds())
                + std::string(RESET));
            
            FD_CLR(c->_accept_fd, &g_conf._save_readfds); // no need to read request since response is same: 503
            FD_SET(c->_accept_fd, &g_conf._save_writefds); // need to know when response can be written in _accept_fd
            c->_response._retry_after = UNAVAILABLE_TIME;
            c->recv_status = Client::COMPLETE;
            _clients_503.push_back(c);
        }
        else
		{
            _clients.push_back(c);
		    LOG_WRT(Logger::INFO, _name + " has now " + std::to_string(_clients.size()) + " clients connected");
        }
        return (1);
    }
}

int Server::recvRequest(Client *c)
{
    int ret = 0;
    errno = 0;
    int bytes;
    
    LOG_WRT(Logger::DEBUG, "--- start recv() ---");
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

    bytes = strlen(c->_buffermalloc);
    ret = recv(c->_accept_fd, c->_buffermalloc + bytes, RECV_BUFFER - bytes, 0);
    bytes += ret;

    LOG_WRT(Logger::DEBUG, "recv() -> ret = " + std::to_string(ret) + " | bytes = " + std::to_string(bytes));
    LOG_WRT(Logger::DEBUG, "c->recv_status = " + std::to_string(c->recv_status));
	if (ret == -1)
    {
		LOG_WRT(Logger::ERROR, "Server::recvRequest -> recv(): " + std::string(strerror(errno)));
        // https://stackoverflow.com/questions/10318191/reading-socket-eagain-resource-temporarily-unavailable
        // EAGAIN does not mean you're disconnected, it just means "there's nothing to read now; try again later".
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
        c->_last_active_time = utils_tmp::get_date();
        LOG_WRT(Logger::DEBUG, "ret > 0");
        c->_buffermalloc[bytes] = '\0';
        if (c->recv_status == Client::HEADER)
        {
            LOG_WRT(Logger::DEBUG, "c->recv_status == HEADER");
			if (strstr(c->_buffermalloc, "\r\n\r\n") != NULL)
			{
                LOG_WRT(Logger::DEBUG, "found \"\\r\\n\\r\\n\"");
            	c->_request._buffer = std::string(c->_buffermalloc, bytes);
                LOG_WRT(Logger::DEBUG, "RAW REQUEST (" + std::to_string(bytes) + "):\n---\n" + std::string(c->_request._buffer) + "---");
            	c->_request.parse(_locations);
                if (c->_request._transfer_encoding == "chunked" || c->_request._content_length >= 0)
				{
                    c->recv_status = Client::BODY;
                    
                    c->_concat_body = std::string(c->_buffermalloc);
                    size_t pos = c->_concat_body.find("\r\n\r\n");
                    c->_concat_body.erase(0, pos + 4);
                    LOG_WRT(Logger::DEBUG, "c->_concat_body = " + c->_concat_body);
                    memset(c->_buffermalloc, 0, RECV_BUFFER + 1);
                }
                else
                    c->recv_status = Client::COMPLETE;       
			}
			else if (bytes >= RECV_BUFFER)
			{
				LOG_WRT(Logger::ERROR, "bytes >= RECV_BUFFER: Not a valid http request");
				return (RET_ERROR);
			}
        }
		if (c->recv_status == Client::BODY)
		{
            LOG_WRT(Logger::DEBUG, "c->recv_status == BODY");
            c->_request.update_body();
        }
		if (c->recv_status == Client::COMPLETE)
		{
            LOG_WRT(Logger::DEBUG, "c->recv_status == COMPLETE");
            LOG_WRT(Logger::DEBUG, "RAW REQUEST (" + std::to_string(bytes) + "):\n---\n" + std::string(c->_request._buffer) + "---");
            FD_SET(c->_accept_fd, &g_conf._save_writefds);          
            c->_request.display();
		}
		if (c->recv_status ==  Client::ERROR)
		{
            LOG_WRT(Logger::DEBUG, "c->recv_status == ERROR");
			c->_is_connected = false;
			return (RET_ERROR);
		}
    }
	return (RET_SUCCESS);
}

int Server::sendResponse(Client *c)
{
    int ret = 0;
    errno = 0;

    if (c->_response._bytes_send == 0)
    {
        c->_response.handle_response(&(c->_request));
        c->_response.format_to_send(&(c->_request));
    	FD_CLR(c->_accept_fd, &g_conf._save_readfds);
    }
    
    if ((ret = send(c->_accept_fd,
                    c->_response._to_send.c_str() + c->_response._bytes_send,
                    c->_response._to_send.length() - c->_response._bytes_send,
                    0)) == -1)
    {
        LOG_WRT(Logger::ERROR, "Server::sendResponse -> send(): " + std::string(strerror(errno)));
        c->_is_connected = false;
        return (0);
    }
    else if (ret >= 0)
    {
        c->_last_active_time = utils_tmp::get_date();

        c->_response._bytes_send += ret;

		LOG_WRT(Logger::INFO, _name + "(" + std::to_string(_port) + ") -> send = OK | ret = " + std::to_string(ret));

        if (ret == 0 || c->_response._bytes_send >= c->_response._to_send.length())
        {
            LOG_WRT(Logger::DEBUG, "sendResponse: c->_response._bytes_send="
                + std::to_string(c->_response._bytes_send)
                + " >= _to_send.length()="
                + std::to_string(c->_response._to_send.length())
                + " -> disconnecting client");
            c->_is_finished = true;
        }
        else
        {
            LOG_WRT(Logger::DEBUG, "sendResponse(): _bytes_send="
                + std::to_string(c->_response._bytes_send) + " < _to_send.length()="
                + std::to_string(c->_response._to_send.length())
                + " -> keep going send()");
        }
    }
    return (1);
}

int Server::handleClientRequest(Client *c)
{
    int ok_read = 0;
    int ok_write = 0;

    LOG_WRT(Logger::DEBUG, "Server::handleClientRequest() of client " + std::to_string(c->_accept_fd));

    if (c->_accept_fd == -1)
        return (ok_read || ok_write);

    if (FD_ISSET(c->_accept_fd, &g_conf._readfds))
    {
        LOG_WRT(Logger::INFO, "reading request of client " + std::to_string(c->_accept_fd));
        if (!recvRequest(c))
            return (0);
        else
            ok_read = 1;
    }
    else
        LOG_WRT(Logger::DEBUG, "reading not set for client " + std::to_string(c->_accept_fd));
    if (FD_ISSET(c->_accept_fd, &g_conf._writefds))
    {
        if (c->recv_status != Client::COMPLETE)
        {
            c->recv_status = Client::COMPLETE;
            LOG_WRT(Logger::DEBUG, "Server::handleClientRequest() passing recv_status to COMPLETE for client " + std::to_string(c->_accept_fd));
        }
        LOG_WRT(Logger::INFO, "sending response to client " + std::to_string(c->_accept_fd));
        if (!sendResponse(c))
            return (0);
        else
            ok_write = 1;
    }
    else
        LOG_WRT(Logger::DEBUG, "writing not set for client " + std::to_string(c->_accept_fd));

    return (ok_read || ok_write);
}
