#include "../../includes/Headers.hpp"

void			Response::post(Request *req)
{
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

		//Set the extention of the file whith mime tab
		if ((fd = open(req->_file.c_str(), O_APPEND | O_WRONLY, 0666)) == -1)
		{
			LOG_WRT(Logger::DEBUG, "UPDATE");
			if ((fd = open(req->_file.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0666)) == -1)
			{
				LOG_WRT(Logger::DEBUG, "CREATE");
				_status_code = INTERNAL_ERROR_500;//error server
				return ;
			}
    	    _status_code = CREATED_201;
			_body = "Ressource created";
		}
		else
		{
			_status_code = OK_200;
			_body = "Ressource updated";
		}
		write(fd, req->_text_body.c_str(), req->_text_body.length());
		//LOG_WRT(Logger::DEBUG, req->_text_body);
		close(fd);	
	}
}
