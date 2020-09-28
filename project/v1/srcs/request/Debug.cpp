#include "../../includes/Headers.hpp"

/*
** Display in logs
*/

void Request::display(void)
{
    size_t i;
    std::stringstream ss1;
    
	ss1 << "PARSED REQUEST:" << std::endl;

    ss1 << " 1) _method             " << _method << std::endl; // 1
    ss1 << " 2) _uri                " << _uri << std::endl; // 2
    ss1 << " 3) _http_version       " << _http_version << std::endl; // 3
    ss1 << " 4) _accept_charset:    "; // 4
    utils_tmp::print_map(ss1, _accept_charset);
	ss1 << " 5) _accept_language:   "; // 5
    utils_tmp::print_map(ss1, _accept_language);
    ss1 << " 6) _authorization      " << _authorization << std::endl; // 6
    ss1 << " 7) _content_language  "; // 7
    utils_tmp::print_map(ss1, _content_language);
    ss1 << " 8) _content_length     " << _content_length << std::endl; // 8
    ss1 << " 9) _content_location   " << _content_location << std::endl; // 9
    ss1 << "10) _content_type       " << _content_type << std::endl; // 10
    ss1 << "11) _date               " << _date << std::endl; // 11
    ss1 << "12) _host               " << _host << std::endl; // 12
    ss1 << "13) _referer            " << _referer << std::endl; // 13
    ss1 << "14) _user_agent         " << _user_agent << std::endl; // 14
    ss1 << "15) _text_body length   " << std::to_string(_text_body.length()) /*: " << _text_body */<< std::endl; // 14
    ss1 << "16) _file               " << _file << std::endl;
    ss1 << "17) _transfer_encoding  " << _transfer_encoding << std::endl;
    ss1 << "18) _keep_alive         " << _keep_alive << std::endl;
	LOG_WRT(Logger::INFO, ss1.str());
}
