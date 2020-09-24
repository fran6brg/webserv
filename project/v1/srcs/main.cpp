#include "../includes/Headers.hpp"

Conf g_conf;

void	shutdown(int sig)
{
	(void)sig;
	LOG_WRT(Logger::INFO, "\33[2K\r" + g_conf._webserv + " status off");
	g_conf._on = false;
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	Server *s;
	Client *c;

	//logger start
	LOG_START(Logger::DEBUG, "OUIOUIOUIO", false);
	
	(void)argc;
	(void)argv;
	signal(SIGINT, shutdown);

	if (/*argc != 2 || */!g_conf.parse(argv[1]))
        return (EXIT_ERROR);

    // loop
    while (g_conf._on)
    {
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
				LOG_WRT(Logger::INFO, std::string(GREEN_C) + "new client on server " + s->_name + std::string(RESET));
				Logger::ChangeFile();
				s->acceptNewClient(); // connexion et création du nouveau client
			}

			// 2
			std::vector<Client*>::iterator it_c = s->_clients.begin();
			for (; it_c != s->_clients.end(); it_c++)
			{
				c = *it_c;
				
				s->handleClientRequest(c);

				if (!c->_is_connected) // si 'ressource temporary unavailable' || si le client est déconnecté
				{
					// if (!c->_retry_after.empty())
					// {
					// 	FD_CLR(c->_accept_fd, &g_conf._save_readfds);
					// 	FD_CLR(c->_accept_fd, &g_conf._save_writefds);
					// 	g_conf.remove_fd(c->_accept_fd);
					// 	close(c->_accept_fd);
					// 	c->_accept_fd = -1;
					// 	s->_client_saved.push_back(c);
					// 	it_c = s->_clients.erase(it_c);
					// }
					// else
					// {
						it_c = s->_clients.erase(it_c);
						delete c;
					// }
          			LOG_WRT(Logger::INFO, s->_name + " has now " + std::to_string(s->_clients.size()) + " client(s) connected");
					if (s->_clients.empty())
						break;
				}
			}
		}
		LOG_WRT(Logger::DEBUG, "---------------\n\n");
    }
    return (EXIT_SUCCESS);
}
