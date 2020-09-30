#include "../includes/Headers.hpp"

Conf g_conf;

void print_clients_of_all_servers(void)
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

int erase_client_from_vector(Server *s, std::vector<Client*> &v, std::vector<Client*>::iterator &it_c)
{
	Client *c = *it_c;
	delete c;
	it_c = v.erase(it_c);
	
	LOG_WRT(Logger::INFO, s->_name + " has now " + std::to_string(v.size()) + " client(s) connected");

	// print_clients_of_all_servers();
	if (v.empty())
	{
		LOG_WRT(Logger::DEBUG, s->_name + " v.empty()");
		return (1);
	}
	else
	{
		LOG_WRT(Logger::DEBUG, s->_name + " il reste encore des clients");
		it_c = v.begin();
	}
	
	LOG_WRT(Logger::DEBUG, s->_name + " go to next client");
	return (0);
}

void	shutdown(int sig)
{
	Server *s;
	Client *c;
	std::vector<Client*>::iterator it_c;
	
	g_conf._on = false;
	LOG_WRT(Logger::INFO, "\33[2K\r" + g_conf._webserv + " deleting clients ...");
	std::vector<Server*>::iterator it_s = g_conf._servers.begin();
	for (; it_s != g_conf._servers.end(); it_s++)
	{
		s = *it_s;
		for (it_c = s->_clients_503.begin(); it_c != s->_clients_503.end(); it_c++)
		{
			c = *it_c;
			if (erase_client_from_vector(s, s->_clients_503, it_c))
				break;
			else
				continue;
		}
		c = NULL;
		for (it_c = s->_clients.begin(); it_c != s->_clients.end(); it_c++)
		{
			c = *it_c;
			if (erase_client_from_vector(s, s->_clients, it_c))
				break;
			else
				continue;
		}
	}
	LOG_WRT(Logger::INFO, "\33[2K\r" + g_conf._webserv + " status off");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	Server *s;
	Client *c;
	std::vector<Client*>::iterator it_c;

	LOG_START(Logger::DEBUG, "", false);
	signal(SIGINT, shutdown);
	if (argc != 2 || !g_conf.parse(argv[1]))
        return (EXIT_ERROR);
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
			// 2 - on itère sur les clients_503 du serveur pour les servir
			// 3 - on itère sur les clients du serveur pour les servir

			// FD_ISSET(): check si le fd est dans le set (de ceux qui sont prêts à être lu, grâce au select).
			// the socket is "readable" if the remote peer (the client) closes it / select() returns if a read() will not block, not only when there's data available (meaning also if EOF)
			// (https://stackoverflow.com/questions/6453149/select-says-socket-is-ready-to-read-when-it-is-definitely-not-actually-is-close)
			if (FD_ISSET(s->_socket_fd, &g_conf._readfds))
			{
				LOG_WRT(Logger::INFO, std::string(GREEN_C) + "new client on server " + s->_name + std::string(RESET));
				// Logger::ChangeFile();
				s->acceptNewClient();
				g_conf._nb_accept_opered += 1;
				LOG_WRT(Logger::INFO, std::string(YELLOW_C) + "_nb_accept_opered = " + std::to_string(g_conf._nb_accept_opered) + std::string(RESET));
			}
	
			for (it_c = s->_clients_503.begin(); it_c != s->_clients_503.end(); it_c++)
			{
				c = *it_c;
				if (!c->_is_connected)
				{
					if (erase_client_from_vector(s, s->_clients_503, it_c))
						break;
					else
						continue ;
				}
				if (!c->_is_finished)
					s->handleClientRequest(c);
				if (c->_is_finished)
						c->reset();
				if (utils_tmp::getSecondsDiff(c->_last_active_time) >= CLIENT_CONNECTION_TIMEOUT)
				{
					LOG_WRT(Logger::DEBUG, "client "
										+ std::to_string(c->_accept_fd)
										+ " TIMEOUT "
										+ std::to_string(CLIENT_CONNECTION_TIMEOUT));
					if (erase_client_from_vector(s, s->_clients_503, it_c))
						break;
					else
						continue ;
				}
			}
			c = NULL;
			for (it_c = s->_clients.begin(); it_c != s->_clients.end(); it_c++)
			{
				c = *it_c;

				if (!c->_is_connected)
				{
					if (erase_client_from_vector(s, s->_clients, it_c))
						break;
					else
						continue ;
				}

				if (!c->_is_finished)
					s->handleClientRequest(c);

				LOG_WRT(Logger::DEBUG, "client " + std::to_string(c->_accept_fd)
									+ " secondsDiff = " + std::to_string(utils_tmp::getSecondsDiff(c->_last_active_time)));

				if (c->_is_finished)
					c->reset();

				if (utils_tmp::getSecondsDiff(c->_last_active_time) >= CLIENT_CONNECTION_TIMEOUT)
				{
					LOG_WRT(Logger::DEBUG, "client "
										+ std::to_string(c->_accept_fd)
										+ " TIMEOUT "
										+ std::to_string(CLIENT_CONNECTION_TIMEOUT));
					if (erase_client_from_vector(s, s->_clients, it_c))
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
