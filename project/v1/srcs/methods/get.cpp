#include "../../includes/Headers.hpp"

void			Response::get(Request *req)
{
	int		language = 0;
	int		charset = 0;
	int		ret = 0;
	if (req->_client->_wfd == -1 && req->_client->_rfd == -1)	
	{
		if (_body != "")
		{
			_status_code = OK_200;
			_content_type[0] = "text/html";
			return ;
		}
		language = set_laguage(req);
		charset = set_charset(req);
	}
	ret = utils_tmp::file_exists(req->_file.c_str());
	if ((req->_method == "GET" 
		&& ((req->_location->_cgi_root != ""
		&& is_extension(req->_file, ".cgi")) 
		|| (req->_location->_php_root != ""
		&& is_extension(req->_file, ".php"))))
		&& ret)
	{
			LOG_WRT(Logger::DEBUG, "get: cgi\n");
			ft_cgi(req);
			req->_body_file = "./www/temp_file";
			req->_is_body_file_header = true;
			_status_code = OK_200;
			_content_type[0] = "text/html";
			_last_modified = get_last_modif(req->_file);
	}
	else if (ret)
	{
		if (req->_client->_wfd == -1 && req->_client->_rfd == -1)	
		{
			req->_client->_rfd = open(req->_file.c_str(), O_RDONLY);
			_last_modified = get_last_modif(req->_file);
			_status_code = OK_200;
		}
		else
		{
			close(req->_client->_rfd);
			req->_client->_rfd = -1;
		}
	}
	else
		not_found(req);
	if (req->_client->_read_ok == 1)
	{
		if (charset)
			unset_extension(req);
		if (language)
			unset_extension(req);
	}
}
