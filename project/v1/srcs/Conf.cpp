#include "../includes/Conf.hpp"

/*
** constructors / destructors
*/

Conf::Conf()
{
   // loop
    _on = true;
    _webserv = "webserv";
    // select() related
    _nfds = -1;
	FD_ZERO(&_readfds);
	FD_ZERO(&_save_readfds);
	FD_ZERO(&_writefds);
	FD_ZERO(&_save_writefds);
	FD_ZERO(&_exceptfds);
	FD_ZERO(&_save_exceptfds);
    _timeout.tv_sec = 5;
	_timeout.tv_usec = 0;
    // debug
    _nb_requests_received = 0;}

/*
** other class methods
*/

int Conf::parse(char *file)
{
	/*************************/

	Config_parser conf(file);
	conf.setup_server(_servers);

	for (size_t i = 0; i < _servers.size(); ++i)
	{
		LOG_WRT(Logger::DEBUG, "server on");
		if (!_servers[i]->start())
        	return (0);
	}

	/************************/
    // création à la main

/*    std::string n1 ("server1");
    Server *s1 = new Server(n1, 8080);
    if (!s1->start())
        return (0);
    _servers.push_back(s1);

    std::string n2 ("server2");
    Server *s2 = new Server(n2, 4443);
    if (!s2->start())
        return (0);
    _servers.push_back(s2);*/

    return (1);
}

void Conf::reset_fd_sets(void)
{
    _readfds = _save_readfds;
    _writefds = _save_writefds;
    _exceptfds = _save_exceptfds;
}

int Conf::get_nfds(void) const
{
    /*
    ** http://manpagesfr.free.fr/man/man2/select.2.html
    **
    ** nfds est le numéro du plus grand descripteur de fichier des 3 ensembles, plus 1.
    */
    // return (_nfds + 1);
    return (*std::max_element(_active_fds.begin(), _active_fds.end()) + 1); // https://stackoverflow.com/questions/9874802/how-can-i-get-the-max-or-min-value-in-a-vector
}

int Conf::get_nb_open_fds(void) const
{
    return (_active_fds.size()); // https://stackoverflow.com/questions/9874802/how-can-i-get-the-max-or-min-value-in-a-vector
}

void Conf::add_fd(int fd)
{
    _active_fds.push_back(fd);
}

void Conf::remove_fd(int fd)
{
    _active_fds.remove(fd);
}

int Conf::run_select(void)
{
    reset_fd_sets(); // la fonction select() exclue les fds qui ne sont pas prêts donc il faut pouvoir reconstituer le pool de fd à chaque tour de boucle
    
    LOG_WRT(Logger::DEBUG, "select(nfds=" + std::to_string(g_conf.get_nfds()) + ", readfds, writefds, NULL, NULL)");
    return (select(get_nfds(), &_readfds, &_writefds, NULL, NULL)); // todo: quid du timeout

    // LOG_WRT(Logger::DEBUG, "select(nfds=" + std::to_string(g_conf.get_nfds()) + ", readfds, writefds, NULL, timeout)");
    // return (select(get_nfds(), &_readfds, &_writefds, NULL, &_timeout)); // Time out is useless

    /*
    ** http://manpagesfr.free.fr/man/man2/select.2.html
    **
    ** Valeur renvoyée : En cas de réussite select() et pselect() renvoient 
    ** le nombre de descripteurs de fichier dans les trois ensembles de descripteurs retournés 
    ** (c'est-à-dire, le nombre total de bits à 1 dans readfds, writefds, exceptfds) 
    ** qui peut être nul si le délai de timeout a expiré avant que quoi que ce soit d'intéressant ne se produise.
    ** Ils retournent -1 s'ils échouent, auquel cas errno contient le code d'erreur ;
    ** les ensembles et timeout ne sont plus définis, ne vous fiez plus à leur contenu après une erreur.
    */  
}