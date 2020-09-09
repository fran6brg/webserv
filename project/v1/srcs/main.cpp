#include "../includes/Headers.hpp"

Conf g_conf;

int ft_strlen(char *str)
{
	int i = -1;
	while (str[++i])
		;
	return (i);
}

void ft_putfd(int fd, char *str)
{
	write(fd, str, ft_strlen(str));
}

void	shutdown(int sig)
{
	(void)sig;
	printf("\33[2K\r%s is off\n", g_conf._webserv.c_str());
	g_conf._on = false;
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	Server *s;
	Client *c;

	//logger start
	LOGGER_START(Logger::DEBUG, "", true);

	//*** test logger ***
	LOGGER_WRITE(Logger::DEBUG, "test debug");
	LOGGER_WRITE(Logger::INFO, "test info");
	LOGGER_WRITE(Logger::ERROR, "test error");
	//*******************
	
	(void)argc;
	(void)argv;
	signal(SIGINT, shutdown);

    // parsing
	if (/*argc != 2 || */!g_conf.parse(/*argv[1]*/))
        return (EXIT_ERROR);

    // loop
    while (g_conf._on)
    {
		printf("\nselect()\n");
		if (g_conf.run_select() == -1)
			break;

		printf("iterating over existing servers ...\n");
        std::vector<Server*>::iterator it_s = g_conf._servers.begin();
		for (; it_s != g_conf._servers.end(); it_s++)
		{
			s = *it_s;
			// pour chaque serveur:
			// 1 - on accepte, s'il y en a une, la demande de connexion du client auprès du serveur it_s (FD_ISSET())
			// 2 - on itère sur les clients du serveur pour les servir


			// 1
			// FD_ISSET(): check si le fd est dans le set (de ceux qui sont prêts à être lu, grâce au select).
			// the socket is "readable" if the remote peer (the client) closes it / select() returns if a read() will not block, not only when there's data available (meaning also if EOF) (https://stackoverflow.com/questions/6453149/select-says-socket-is-ready-to-read-when-it-is-definitely-not-actually-is-close)
			if (FD_ISSET(s->_socket_fd, &g_conf._readfds))
			{
				printf("fd %i is set <=> incoming connection from a client\n", s->_socket_fd);
				s->acceptNewClient(); // connexion et création du nouveau client
			}

			// 2
			std::vector<Client*>::iterator it_c = s->_clients.begin();
			for (; it_c != s->_clients.end(); it_c++)
			{
				c = *it_c;
				s->handleClientRequest(c);
				if (!c->_is_connected) // on supprime le client
				{
					it_c = s->_clients.erase(it_c);
			        printf("%s has now %lu clients connected\n", s->_name.c_str(), s->_clients.size());
					if (s->_clients.empty())
						break;
				}
			}
		}
    }
    return (EXIT_SUCCESS);
}