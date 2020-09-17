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

		//Set the extention of the file whith mime tab ?
		if (utils_tmp::file_exists(req->_file.c_str()))
		{
			if ((fd = open(req->_file.c_str(), O_APPEND|O_WRONLY|O_NONBLOCK, 0666)) == -1)
			{
				_status_code = INTERNAL_ERROR_500;
				return ;
			}
			_status_code = OK_200;
			_body = "Ressource updated";
		}
		else
		{
			if ((fd = open(req->_file.c_str(), O_CREAT|O_APPEND|O_WRONLY|O_NONBLOCK, 0666)) == -1)
			{
				_status_code = INTERNAL_ERROR_500;
				return ;
			}
			_status_code = CREATED_201;
			_body = "Ressource created";
		}
		//protect write
		write(fd, req->_text_body.c_str(), req->_text_body.length());
		LOG_WRT(Logger::DEBUG, req->_text_body);
		close(fd);	
	}
}
