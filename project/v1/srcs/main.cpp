#include "../includes/Headers.hpp"

Conf g_conf;

void	shutdown(int sig)
{
	(void)sig;
	LOG_WRT(Logger::INFO, g_conf._webserv + " status off");
	g_conf._on = false;
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	Server *s;
	Client *c;

	//logger start
	LOG_START(Logger::DEBUG, "", false);
	
	(void)argc;
	(void)argv;
	signal(SIGINT, shutdown);

	if (/*argc != 2 || */!g_conf.parse(argv[1]))
        return (EXIT_ERROR);

    // loop
    while (g_conf._on)
    {
		printf("\nselect(nfds=%i, readfds, writefds, NULL, timeout)\n", g_conf.get_nfds());

		if (g_conf.run_select() == -1)
			break;

		LOG_WRT(Logger::DEBUG, "iterating over existing servers ...");
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
				LOG_WRT(Logger::INFO, "New client=" + std::to_string( s->_socket_fd));
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
					delete c;
          			LOG_WRT(Logger::INFO, s->_name + " has now " + std::to_string(s->_clients.size()) + " clients connected");
					if (s->_clients.empty())
						break;
				}
			}
		}
    }
    return (EXIT_SUCCESS);
}
