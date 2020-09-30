#include "../../includes/Headers.hpp"

void			Response::put(Request *req)
{
	LOG_WRT(Logger::DEBUG, "inside put\n");

	std::ifstream f1(req->_file);
	if (f1.good())
	{
        _status_code = OK_200;
		LOG_WRT(Logger::DEBUG, "put: file exists, is good\n");
    }
	else
	{
        _status_code = CREATED_201;
		_body = "Ressource created";
		LOG_WRT(Logger::DEBUG, "put: file created\n");
    }
	f1.close();

	std::ofstream f2(req->_file);
	if (f2.good())
	{
		LOG_WRT(Logger::DEBUG, "put: writing req->text_body (len=" + std::to_string(req->_text_body.length()) + ") inside _file\n");
		f2 << req->_text_body;
    }
	else
	{
	    LOG_WRT(Logger::DEBUG, "put: failed to write body inside _file: \"" + std::string(strerror(errno)) + "\" -> 404\n");
		not_found(req);
    }
	f2.close();
}