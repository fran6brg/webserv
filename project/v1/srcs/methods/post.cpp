#include "../../includes/Headers.hpp"

void		Response::post(Request *req)
{
	int			fd;
	int 		ret;

	if (((req->_location->_cgi_root != "" && is_extension(req->_file, req->_location->_cgi))
		|| (req->_location->_php_root != "" && is_extension(req->_file, "php"))))
	{
		if (req->_client->_wfd == -1 && req->_client->_rfd == -1)
			ft_cgi(req);
		else
		{
			// FAIRE UN FT EXTRACT HEADER !!!!!!!!!!
			_content_type[0] = "text/html";
			_status_code = OK_200;
			LOG_WRT(Logger::DEBUG, "Response::post(): avant get_buffer");
			if (utils_tmp::extract_body(_body) == -1)
			{
				LOG_WRT(Logger::DEBUG, "Response::post(): extract_body -> error");
				remove("./www/temp_file");
				close(req->_client->_wfd);
				close(req->_client->_rfd);
				req->_client->_wfd = -1;
				req->_client->_rfd = -1;
				_status_code = INTERNAL_ERROR_500;
				return ;
			}
			remove("./www/temp_file");
			close(req->_client->_wfd);
			close(req->_client->_rfd);
			req->_client->_wfd = -1;
			req->_client->_rfd = -1;
		}
	}
	else
	{
		if (req->_client->_wfd == -1 && req->_client->_rfd == -1)
		{
			if (utils_tmp::file_exists(req->_file.c_str()))
			{
				if ((req->_client->_wfd  = open(req->_file.c_str(), O_APPEND|O_WRONLY|O_NONBLOCK, 0666)) == -1)
				{
					_status_code = INTERNAL_ERROR_500;
					return ;
				}
				_status_code = OK_200;
				_body = "Ressource updated";
			}
			else
			{
				if ((req->_client->_wfd = open(req->_file.c_str(), O_CREAT|O_APPEND|O_WRONLY|O_NONBLOCK, 0666)) == -1)
				{
					_status_code = INTERNAL_ERROR_500;
					return ;
				}
				_status_code = CREATED_201;
				_body = "Ressource created";
			}

		}
		else
		{
			LOG_WRT(Logger::DEBUG, "req->_text_body(" + std::to_string(req->_text_body.length()) + ")= " + req->_text_body);
			close(req->_client->_wfd);
			req->_client->_wfd = -1;
		}
	}
}
