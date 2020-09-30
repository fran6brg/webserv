#include "../../includes/Headers.hpp"

void			Response::put(Request *req)
{
	LOG_WRT(Logger::DEBUG, "inside put\n");
	int		ret = 0;

	if (req->_client->_wfd == -1 && req->_client->_rfd == -1)
	{
		ret = utils_tmp::file_exists(req->_file.c_str());
		if (ret)
		{
 	       _status_code = OK_200;
			LOG_WRT(Logger::DEBUG, "put: file exists, is good\n");
		}
		else
		{
			req->_client->_wfd = open(req->_file.c_str(), O_CREAT|O_APPEND|O_WRONLY|O_NONBLOCK, 0666);
			_status_code = CREATED_201;
			_body = "Ressource created";
			LOG_WRT(Logger::DEBUG, "put: file created\n");
			close(req->_client->_wfd);
 		}
		
		ret = utils_tmp::file_exists(req->_file.c_str());
		if (ret)
		{
			req->_client->_wfd  = open(req->_file.c_str(), O_APPEND|O_WRONLY|O_NONBLOCK, 0666);
			FD_SET(req->_client->_wfd, &g_conf._save_writefds);
			g_conf.add_fd(req->_client->_wfd);
			LOG_WRT(Logger::DEBUG, "put: writing req->text_body (len=" + std::to_string(req->_text_body.length()) + ") inside _file\n");
		}
		else
		{
		    LOG_WRT(Logger::DEBUG, "put: failed to write body inside _file: \"" + std::string(strerror(errno)) + "\" -> 404\n");
			not_found(req);
 		}
	}
	else
	{
		FD_CLR(req->_client->_wfd, &g_conf._save_writefds);
		g_conf.remove_fd(req->_client->_wfd);
		close(req->_client->_wfd);
		req->_client->_wfd = -1;
	}
}