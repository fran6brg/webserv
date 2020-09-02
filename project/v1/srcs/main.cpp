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


int main(int argc, char *argv[])
{
	Server *s;
	Client *c;

    // parsing
	(void)argc;
	(void)argv;
	if (/*argc != 2 || */!g_conf.parse(/*argv[1]*/))
        return (EXIT_ERROR);

    // loop
    while (g_conf._on)
    {
		printf("run_select()\n");
		if (g_conf.run_select() == -1)
			break;

		printf("iterating over existing servers\n");
        std::vector<Server*>::iterator it_s = g_conf._servers.begin();
		for (; it_s != g_conf._servers.end(); it_s++)
		{
			s = *it_s;
			// pour chaque serveur:
			// 1 - on accepte, s'il y en a une, la demande de connexion du client auprès du serveur it_s (FD_ISSET())
			// 2 - on itère sur les clients du serveur pour les servir


			// 1
			if (FD_ISSET(s->_socket_fd, &g_conf._readfds)) // check si le fd est dans le set (de ceux qui sont prêts à être lu, grâce au select).
			{
				printf("fd %i is set\n", s->_socket_fd);
				s->connectClient(); // connexion et création du nouveau client
			}

			// 2
			std::vector<Client*>::iterator it_c = s->_clients.begin();
			printf("%s has now %lu clients to serve\n", s->_name.c_str(), s->_clients.size());
			for (; it_c != s->_clients.end(); it_c++)
			{
				c = *it_c;
				if (c->_is_connected)
					s->handleClientRequest(c);
				else if (s->_clients.size() > 1)
					it_c = s->_clients.erase(it_c); // sinon on supprime le client (mais bug lorsqu'il n'y en a qu'un à supp)
			}
		}
    }
    return (EXIT_SUCCESS);
}