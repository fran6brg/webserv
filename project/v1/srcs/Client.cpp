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
    _ip = inet_ntoa(addr.sin_addr); // todo: à recoder si non authorisée
	_port = htons(addr.sin_port); // todo: à recoder si non authorisée
	_request._client = this;
	// select():
	// FD_SET(_accept_fd, &g_conf._save_readfds);
	FD_SET(_accept_fd, &g_conf._readfds);
	g_conf.set_nfds(_accept_fd, 1);
}

Client::~Client()
{
	std::cout << "closing connection for client " << _accept_fd << std::endl;
	// FD_CLR(_accept_fd, &g_conf._save_readfds);
	FD_CLR(_accept_fd, &g_conf._readfds);
	g_conf.set_nfds(_accept_fd, 0);
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