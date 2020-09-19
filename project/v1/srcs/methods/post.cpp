#include "../../includes/Headers.hpp"

void		Response::post(Request *req)
{
	std::string buff;
	int			fd;

	if ((req->_location->_cgi_root != "" ||
		(req->_location->_php_root != "" && is_php(req->_file))))//plus précis pour le path
	{
		ft_cgi(req); // if not workin = internal error
		//get_cgi_ret(); // stoi impossible si format error
		if ((fd = open("./www/temp_file", O_RDONLY|O_NONBLOCK, 0666)) == -1)//mettre dans read file
		{
			_status_code = INTERNAL_ERROR_500;
			return ;
		}
		buff = utils_tmp::read_file(fd);
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
	//protect write
	//LOG_WRT(Logger::DEBUG, req->_text_body);
	close(fd);
}
