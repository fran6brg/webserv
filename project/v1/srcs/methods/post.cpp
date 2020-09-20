#include "../../includes/Headers.hpp"

void		Response::post(Request *req)
{
	int			fd;
	std::string buff;

	if ((req->_location->_cgi_root != "" ||
		(req->_location->_php_root != "" && is_php(req->_file))))//plus précis pour le path
	{
		ft_cgi(req);// check if error
		get_cgi_ret();
		if (utils_tmp::read_file(fd, "./www/temp_file", buff) == -1)
		{
			_status_code = INTERNAL_ERROR_500;
			remove("./www/temp_file");
			return ;
		}
		_body = utils_tmp::extract_body(buff);
		remove("./www/temp_file");
	}
	else
	{
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
		write(fd, req->_text_body.c_str(), req->_text_body.length());
		LOG_WRT(Logger::DEBUG, "req->_text_body(" + std::to_string(req->_text_body.length()) + ")= " + req->_text_body);
		close(fd);
	}
}

/*void		Response::post(Request *req)
{
	std::string buff;
	int			fd;

	if ((req->_location->_cgi_root != "" ||
		(req->_location->_php_root != "" && is_php(req->_file))))//plus précis pour le path
	{
		ft_cgi(req);
		//get_cgi_ret(); // Parse if error get status code ?
		if (utils_tmp::read_file(fd, "./www/temp_file", buff) == -1)
		{
			_status_code = INTERNAL_ERROR_500;
			remove("./www/temp_file");
			return ;
		}
		remove("./www/temp_file");
	}
	else
		buff = req->_text_body;

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
	write(fd, buff.c_str(), buff.length());
	LOG_WRT(Logger::DEBUG, "buff(" + std::to_string(buff.length()) + ")= " + buff);
	//protect write
	close(fd);
}*/
