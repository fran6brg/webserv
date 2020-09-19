#include "../../includes/Headers.hpp"

void		Response::post(Request *req)
{
	std::string file;
	int			fd;

	if ((req->_location->_cgi_root != "" ||
		(req->_location->_php_root != "" && is_php(file))))//plus précis
	{
		ft_cgi(req); // if not workin = internal error
		//get_cgi_ret();

		//------------
		std::ifstream temp("./www/temp_file");
		std::string buffer((std::istreambuf_iterator<char>(temp)), std::istreambuf_iterator<char>());

//		_body = buffer;           								// --
		req->_text_body = buffer; 								// ++
		
		remove("./www/temp_file");
		//-----------

		LOG_WRT(Logger::DEBUG, "=====>body: " + _body);
//		write(fd, _body.c_str(), _body.length());				// --
	}
//	else														// -- On ouvre le fichier meme si cgi
//	{
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
//	}
	//protect write
	//LOG_WRT(Logger::DEBUG, req->_text_body);
	close(fd);
}
