#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Client::Client(Server *server, int accept_fd, struct sockaddr_in addr):
    _server(server), _accept_fd(accept_fd), _is_connected(true)
{
	// convertit l'adresse Internet de l'hôte cp depuis la notation IPv4 décimale pointée
	// vers une forme binaire (dans l'ordre d'octet du réseau),
	// et la stocke dans la structure pointée
	// http://manpagesfr.free.fr/man/man3/inet.3.html
    _ip = inet_ntoa(addr.sin_addr); // todo: à recoder si non autorisée
	_port = htons(addr.sin_port); // todo: à recoder si non autorisée
	_request._client = this;
	// select():
	FD_SET(_accept_fd, &g_conf._save_readfds);
	// FD_SET(_accept_fd, &g_conf._readfds);
	// FD_SET(_accept_fd, &g_conf._save_writefds);
	// FD_SET(_accept_fd, &g_conf._writefds);

	g_conf.add_fd(_accept_fd);

	_buffermalloc = (char *)malloc(sizeof(char) * (RECV_BUFFER + 1));
	memset((void *)_buffermalloc, 0, RECV_BUFFER + 1);

	recv_status = HEADER;
	_line_size = -1;

	LOG_WRT(Logger::INFO, std::string(BLUE_C) + "client constructor " + _ip + ":" + std::to_string(_port) + std::string(RESET));
}

Client::~Client()
{
	LOG_WRT(Logger::INFO, std::string(RED_C) + "closing connection for client " + std::to_string(_accept_fd) + std::string(RESET));
	FD_CLR(_accept_fd, &g_conf._save_readfds);
	// FD_CLR(_accept_fd, &g_conf._readfds);
	FD_CLR(_accept_fd, &g_conf._save_writefds);
	// FD_CLR(_accept_fd, &g_conf._writefds);
	g_conf.remove_fd(_accept_fd); 
	close(_accept_fd);
	_accept_fd = -1;
}

/*
** other class methods
*/

int Client::parse_request(void)
{
    _request.init();
    return (1);
}