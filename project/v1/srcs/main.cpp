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
        std::vector<Server>::iterator it_s = g_conf._servers.begin();
		for (; it_s != g_conf._servers.end(); it_s++)
		{
			// pour chaque serveur:
			// - on accepte, s'il y en a une, la demande de connexion du client auprès du serveur it_s (FD_ISSET())
			// - on itère sur les clients du serveur pour les servir

			if (FD_ISSET(it_s->_socket_fd, &g_conf._readfds)) // check si le fd est dans le set (de ceux qui sont prêts à être lu, grâce au select).
			{
				printf("fd %i is set\n", it_s->_socket_fd);
				it_s->connectClient(); // connexion et création du nouveau client
			}
			// pour chaque client dudit server:
			std::vector<Client*>::iterator it_c = it_s->_clients.begin();
			printf("%s has now %lu clients to serve\n", it_s->_name.c_str(), it_s->_clients.size());
			for (; it_c != it_s->_clients.end(); it_c++)
			{
				c = *it_c;
				if (c->_accept_fd != -1)
					it_s->handleClientRequest(c);
			}
		}
    }
    return (EXIT_SUCCESS);
}