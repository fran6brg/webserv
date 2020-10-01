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
	_wfd = -1;
	_rfd = -1;
	_read_ok = 1;
	LOG_WRT(Logger::INFO, std::string(BLUE_C) + "client constructor " + _ip + ":" + std::to_string(_port) + std::string(RESET));
}

Client::~Client()
{
	LOG_WRT(Logger::INFO, std::string(RED_C) + "Destructor of client " + std::to_string(_accept_fd) + std::string(RESET));
	FD_CLR(_accept_fd, &g_conf._save_readfds);
	FD_CLR(_accept_fd, &g_conf._readfds);
	FD_CLR(_accept_fd, &g_conf._save_writefds);
	FD_CLR(_accept_fd, &g_conf._writefds);
	g_conf.remove_fd(_accept_fd); 
	free(_buffermalloc);
	close(_accept_fd);
	_accept_fd = -1;
}

/*
** other class methods
*/

void Client::reset(void)
{
	LOG_WRT(Logger::INFO, std::string(BLUE_C) + "reset() client " + _ip + ":" + std::to_string(_port) + std::string(RESET));
	
	_is_finished = false;
    _request.reset();
    _response.reset();
	_concat_body.clear();
	FD_SET(_accept_fd, &g_conf._save_readfds);
	FD_CLR(_accept_fd, &g_conf._readfds);
	FD_CLR(_accept_fd, &g_conf._save_writefds);
	FD_CLR(_accept_fd, &g_conf._writefds);
	
	// FD_CLR(_rfd, &g_conf._save_readfds);
	// g_conf.remove_fd(_rfd); 
	// FD_CLR(_wfd, &g_conf._save_writefds);
	// g_conf.remove_fd(_wfd);

	memset((void *)_buffermalloc, 0, RECV_BUFFER + 1);
	recv_status = HEADER;
	_line_size = -1;
}

void	Client::write_file()
{
	int	ret;

	LOG_WRT(Logger::DEBUG, "IS SET WRITE");
	ret = write(_wfd, _request._text_body.c_str(), _request._text_body.length());
	if (ret == -1)
	{
		_response._status_code = INTERNAL_ERROR_500;
		return ;
	}
	else if (ret == 0)
		;
}

void	Client::read_file(std::string &buff)
{
	int	ret;
	int	status;
	char buffer[BUFFER_SIZE + 1];

	LOG_WRT(Logger::DEBUG, "rfd = " + std::to_string(_rfd));
	LOG_WRT(Logger::DEBUG, "wfd = " + std::to_string(_wfd));

	waitpid((pid_t)_pid, (int *)&status, 0);
	
	ret = read(_rfd, buffer, BUFFER_SIZE);
	if (ret == -1)
	{
		_response._status_code = INTERNAL_ERROR_500;
		return ;	
	}
	
	if (ret == 0)
		_read_ok = 1;
	else if (ret > 0)
		_read_ok = 0;
	
	if (ret >= 0 && !(_request._body_file.empty()))
	{
		LOG_WRT(Logger::DEBUG, "RET1 =  [" + std::to_string(ret) + "]");
		buffer[ret] = '\0';
		_response.build_chunked(_request, buffer, ret);
		_read_ok = 1;
		_rfd = -1;
	}
	else
	{
		LOG_WRT(Logger::DEBUG, "RET2 =  [" + std::to_string(ret) + "]");
		buffer[ret] = '\0';
		buff += std::string(buffer);
	}

}
