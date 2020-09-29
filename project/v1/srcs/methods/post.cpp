#include "../../includes/Headers.hpp"

void		Response::post(Request *req)
{
	int			fd;
	int 		ret;

	if (((req->_location->_cgi_root != "" && is_extension(req->_file, req->_location->_cgi))
		|| (req->_location->_php_root != "" && is_extension(req->_file, "php"))))
	{
		ft_cgi(req);
		get_cgi_ret(req);
		LOG_WRT(Logger::DEBUG, "Response::post(): avant get_buffer");
		
		if (utils_tmp::get_buffer("./www/temp_file", _body) == -1)
		{
            LOG_WRT(Logger::DEBUG, "Response::post(): get_buffer -> error");
            _status_code = INTERNAL_ERROR_500;
            remove("./www/temp_file");
            return ;
        }
		remove("./www/temp_file");
		if (utils_tmp::extract_body(_body) == -1)
		{
			LOG_WRT(Logger::DEBUG, "Response::post(): extract_body -> error");
			_status_code = INTERNAL_ERROR_500;
			return ;
		}
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
		ret = write(fd, req->_text_body.c_str(), req->_text_body.length());
		if (ret == -1)
			_status_code = INTERNAL_ERROR_500;
		else if (ret == 0)
			;
		LOG_WRT(Logger::DEBUG, "req->_text_body(" + std::to_string(req->_text_body.length()) + ")= " + req->_text_body);
		close(fd);
	}
}
