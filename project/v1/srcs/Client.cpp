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
    _ip = inet_ntoa(addr.sin_addr);
	_port = htons(addr.sin_port);
	_request._client = this;
	_response._client = this;
	FD_SET(_accept_fd, &g_conf._save_readfds);
	g_conf.add_fd(_accept_fd);
	_buffermalloc = (char *)malloc(sizeof(char) * (RECV_BUFFER + 1));
	memset((void *)_buffermalloc, 0, RECV_BUFFER + 1);
	recv_status = HEADER;
	_line_size = -1;
	_last_active_time = utils_tmp::get_date();
	LOG_WRT(Logger::INFO, std::string(BLUE_C) + "client constructor " + _ip + ":" + std::to_string(_port) + std::string(RESET));
}

Client::~Client()
{
	LOG_WRT(Logger::INFO, std::string(RED_C) + "closing connection for client " + std::to_string(_accept_fd) + std::string(RESET));
	FD_CLR(_accept_fd, &g_conf._save_readfds);
	FD_CLR(_accept_fd, &g_conf._readfds);
	FD_CLR(_accept_fd, &g_conf._save_writefds);
	FD_CLR(_accept_fd, &g_conf._writefds);
	free(_buffermalloc);
	g_conf.remove_fd(_accept_fd); 
	close(_accept_fd);
	_accept_fd = -1;
}

/*
** other class methods
*/

void Client::reset(void)
{
	_is_finished = false;
    _request.reset();
    _response.init();
	FD_SET(_accept_fd, &g_conf._save_readfds);
	FD_CLR(_accept_fd, &g_conf._readfds);
	FD_CLR(_accept_fd, &g_conf._save_writefds);
	FD_CLR(_accept_fd, &g_conf._writefds);	
	memset((void *)_buffermalloc, 0, RECV_BUFFER + 1);
	recv_status = HEADER;
	_line_size = -1;
	LOG_WRT(Logger::INFO, std::string(BLUE_C) + "reset() client " + _ip + ":" + std::to_string(_port) + std::string(RESET));
}