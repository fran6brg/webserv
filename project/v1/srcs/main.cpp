#include "../includes/Headers.hpp"

Conf g_conf;

void	shutdown(int sig)
{
	(void)sig;
	LOG_WRT(Logger::INFO, "\33[2K\r" + g_conf._webserv + " status off");
	g_conf._on = false;
	exit(EXIT_SUCCESS);
}

void print_clients()
{
    Server *s;
    Client *c;
    LOG_WRT(Logger::DEBUG, "-------------------------------");
    std::vector<Server*>::iterator it_s = g_conf._servers.begin();
    for (; it_s != g_conf._servers.end(); it_s++)
    {
        s = *it_s;
        LOG_WRT(Logger::DEBUG, "SERVER : " + s->_name);
        std::vector<Client*>::iterator it_c = s->_clients.begin();
        for (; it_c != s->_clients.end(); it_c++)
        {
            c = *it_c;
            LOG_WRT(Logger::DEBUG, "<==>CLIENT fd=" + std::to_string(c->_accept_fd));
        }
    }
    LOG_WRT(Logger::DEBUG, "-------------------------------");
}

int erase_client(Server *s, Client *c, std::vector<Client*>::iterator &it_c)
{
	it_c = s->_clients.erase(it_c);
	delete c;
	LOG_WRT(Logger::INFO, s->_name + " has now " + std::to_string(s->_clients.size()) + " client(s) connected");

	print_clients();
	if (s->_clients.empty())
	{
		LOG_WRT(Logger::INFO, s->_name + " s->_clients.empty()");
		return (1);
	}
	else
	{
		LOG_WRT(Logger::INFO, s->_name + " il reste encore des clients");
		it_c = s->_clients.begin();
	}
	LOG_WRT(Logger::INFO, s->_name + " go to next client");
	return (0);
}


int main(int argc, char *argv[])
{
	Server *s;
	Client *c;

	//logger start
	LOG_START(Logger::DEBUG, "", false);
	
	signal(SIGINT, shutdown);

	if (argc != 2 || !g_conf.parse(argv[1]))
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
				// Logger::ChangeFile();
				if (g_conf.get_nb_open_fds() < OPEN_MAX)
				{
					s->acceptNewClient(); // connexion et création du nouveau client
					g_conf._nb_requests_received += 1;
					LOG_WRT(Logger::INFO, std::string(YELLOW_C) + "_nb_requests_received = " + std::to_string(g_conf._nb_requests_received) + std::string(RESET));

				}
		//		else
				//	s->saveNewClient(); // ?
				
			}
	
			// 2
			std::vector<Client*>::iterator it_c = s->_clients.begin();
			for (; it_c != s->_clients.end(); it_c++)
			{
				c = *it_c;

				if (!c) // what ?
					break;

				if (!c->_is_connected)
				{
					if (erase_client(s, c, it_c))
						break;
					else
						continue ;
				}

				if (!c->_is_finished)
					s->handleClientRequest(c);

				LOG_WRT(Logger::DEBUG, "client "
										+ std::to_string(c->_accept_fd)
										+ " secondsDiff = "
										+ std::to_string(utils_tmp::getSecondsDiff(c->_last_active_time)));

				if (c->_is_finished) // si on a fini d'envoyer la réponse
					c->reset();

				if (utils_tmp::getSecondsDiff(c->_last_active_time) >= 10 /*CLIENT_CONNECTION_TIMEOUT*/)
				{
					LOG_WRT(Logger::DEBUG, "client "
										+ std::to_string(c->_accept_fd)
										+ " TIMEOUT "
										+ std::to_string(10 /*CLIENT_CONNECTION_TIMEOUT*/));
					if (erase_client(s, c, it_c))
						break;
					else
						continue ;
				}
			}
		}
		LOG_WRT(Logger::DEBUG, "---------------\n\n");
    }
    return (EXIT_SUCCESS);
}
