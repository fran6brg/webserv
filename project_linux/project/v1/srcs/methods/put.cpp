#include "../../includes/Headers.hpp"

void			Response::put(Request *req)
{
	LOG_WRT(Logger::DEBUG, "inside put\n");

    // if (req->_content_length > 0)
    // {
	//     LOG_WRT(Logger::DEBUG, "inside put: BAD_REQUEST_400");
    //     _status_code = BAD_REQUEST_400;
    //     return ;
    // }

    // if (req->_content_length != req->_body_length)
    // {
	//     LOG_WRT(Logger::DEBUG, "inside put: REQUEST_ENTITY_TOO_LARGE_413: " + std::to_string(req->_content_length) + " vs " +  std::to_string(req->_body_length));
    //     _status_code = REQUEST_ENTITY_TOO_LARGE_413;
    //     return ;
    // }

	std::ifstream f1(req->_file);
	if (f1.good()) // test if _file already exists
	{
        _status_code = OK_200;
		LOG_WRT(Logger::DEBUG, "put: file exists, is good\n");
    }
	else // _file does not exist
	{
        _status_code = CREATED_201;
		// _location = req->_uri; // déjà fait dans format_to_send()
		_body = "Ressource created";
		LOG_WRT(Logger::DEBUG, "put: file created\n");
    }
	f1.close();

	std::ofstream f2(req->_file);
	if (f2.good())
	{
        // if (req->_content_type == "application/x-www-form-urlencoded")
		// {
		//     LOG_WRT(Logger::DEBUG, "put: writing req->body inside _file\n");
		// 	f2 << req->_body[0].second.substr(0, req->_content_length);
		// }
        // else // simple text, no query string formated
		// {
			LOG_WRT(Logger::DEBUG, "put: writing req->text_body (len=" + std::to_string(req->_text_body.length()) + ") inside _file\n");
			f2 << req->_text_body;
		// }
    }
	else
	{
	    LOG_WRT(Logger::DEBUG, "put: failed to write body inside _file: \"" + std::string(strerror(errno)) + "\" -> 404\n");
		not_found(req);
    }
	f2.close();
}