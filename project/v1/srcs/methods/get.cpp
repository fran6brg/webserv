#include "../../includes/Headers.hpp"

void			Response::get(Request *req)
{
	int		language = 0;
	int		charset = 0;
	int		ret = 0;

	LOG_WRT(Logger::DEBUG, "inside GET");
	if (req->_client->_wfd == -1 && req->_client->_rfd == -1)
	{
		LOG_WRT(Logger::DEBUG, "1er passage dans get");
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
	LOG_WRT(Logger::DEBUG, "req->_file=-" + req->_file + "- | ret = " + std::to_string(ret));
	// CGI
	if ((req->_method == "GET" 
		&& ((req->_location->_cgi_root != ""
		&& is_extension(req->_file, ".cgi")) 
		|| (req->_location->_php_root != ""
		&& is_extension(req->_file, ".php"))))
		&& ret)
	{
			LOG_WRT(Logger::DEBUG, "CGI");
			ft_cgi(req);
			req->_body_file = "./www/temp_file";
			req->_is_body_file_header = true;
			_status_code = OK_200;
			_content_type[0] = "text/html";
			_last_modified = get_last_modif(req->_file);
	}
	// Pas de CGI
	else if (ret)
	{
		LOG_WRT(Logger::DEBUG, "pas de CGI");
		if (req->_client->_wfd == -1 && req->_client->_rfd == -1)	
		{
			req->_client->_rfd = open(req->_file.c_str(), O_RDONLY);
			FD_SET(req->_client->_rfd, &g_conf._save_readfds);
			g_conf.add_fd(req->_client->_rfd);
			_last_modified = get_last_modif(req->_file);
			_status_code = OK_200;
		}
		else
		{
			FD_CLR(req->_client->_rfd, &g_conf._save_readfds);
			g_conf.remove_fd(req->_client->_rfd);
			close(req->_client->_rfd);
			req->_client->_rfd = -1;
		}
	}
	else
	{
		LOG_WRT(Logger::DEBUG, "not found");
		not_found(req);
	}
	if (req->_client->_read_ok == 1)
	{
		if (charset)
			unset_extension(req);
		if (language)
			unset_extension(req);
	}
}
