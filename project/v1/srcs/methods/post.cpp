#include "../../includes/Headers.hpp"

void			Response::post(Request *req)
{(void)req;/*
	LOG_WRT(Logger::DEBUG, "I am right !!!!!!!!!!");
		// add is_cgi(req->_file)
	if ((req->_location->_cgi_root != "" ||
		(req->_location->_php_root != "" && is_php(req->_file))))
	{
		ft_cgi(req);
		get_cgi_ret();
		// faire quoi tu retour du cgi
	}
	else
	{
		int fd;

		if ((fd = open(req->_file.c_str(), O_APPEND)) == -1)
		{
			//create file
			if ((fd = open(req->_file.c_str(), O_CREAT|O_APPEND, 0666)) == -1)
			{
				_status_code = INTERNAL_ERROR_500;//error server
				return ;
			}
    	    _status_code = CREATED_201;
			_body = "Ressource created";//why
		}
		else
		{
			_status_code = OK_200;
			_body = "Ressource updated";//why
		}
		write(fd, req->_text_body.c_str(), req->_text_body.length());
		close(fd);
	}*/
}
