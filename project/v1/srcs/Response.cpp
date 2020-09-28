#include "../includes/Headers.hpp"

/*
** constructors / destructors
*/

Response::Response(void)
{
    init();
}


/*
** private class methods
*/

void Response::init(void)
{
    // Status Line
    _http_version.clear();
    _status_code = 0;
    _reason_phrase.clear();
    // Request Headers, dans l'ordre du sujet
    _allow.clear();
    _content_language.clear();
    _content_length = 0; // est-ce qu'on peut avoir un content_length de 0 ?
    _content_location.clear();
    _content_type.clear();
    _last_modified.clear();
    _location.clear();
    _date.clear();
    _retry_after = -1;
    _server.clear();
    _transfer_encoding.clear();
    _www_authenticate.clear();
    // Request body
    _body.clear();
    // Content to write in fd
    _to_send.clear();
    // Other headers
    _keep_alive.clear();
    // Other
    _bytes_send = 0;
}

int Response::concat_to_send(void)
{
    std::stringstream ss;
    std::string response_headers;
    size_t i;
    
    // 1. Status Line
    ss << _http_version << " ";
    ss << _status_code << " ";
    ss << _reason_phrase << "\r\n";

    // 2. Request Headers, dans l'ordre du sujet
    // if (!_content_language.empty())     { ss << "content_language: " << _content_language << "\r\n"; }
    // if (!_date.empty())                 { ss << "Date: " << _date << "\r\n"; }
    if (_content_length >= 0)
        ss << "Content-Length: " << _content_length << "\r\n";
    // if (!_content_location.empty())     { ss << "content_location: " << _content_location << "\r\n"; }
    if (!_allow.empty())                { ss << "allow: " << _allow << "\r\n"; }
    // if (!_content_type.empty())         { ss << "Content-Type: " << _content_type << "\r\n"; }
    if (!_last_modified.empty())        { ss << "Last-Modified: " << _last_modified << "\r\n"; }
    if (_retry_after >= 0)              { ss << "Retry-After: " << std::to_string(_retry_after) << "\r\n"; }
    if (!_location.empty() && _status_code == 201)  { ss << "Location: " << _location << "\r\n"; }
    if (!_date.empty())                 { ss << "Date: " << _date << "\r\n"; }
    if (!_server.empty())               { ss << "Server: " << _server << "\r\n"; }
    // if (!_transfer_encoding.empty())    { ss << "transfer_encoding: " << _transfer_encoding << "\r\n"; }
    // if (!_www_authenticate.empty())     { ss << "www_authenticate: " << _www_authenticate << "\r\n"; }
    if (!_content_type.empty())
	{
		ss << "Content-Type:";
        i = 0;
    	while (i < _content_type.size())
    	{
    	    ss << " " << _content_type[i];
    	    if (i + 1 < _content_type.size())
    	        ss << ";";
            i++;
    	}
    	ss << "\r\n";
	}

    // 3. Request body
    // ss << "\r\n\r\n"; // double or simple CRLF after the headers ?
    ss << "\r\n"; // CRLF et non pas \n\n https://stackoverflow.com/questions/29131727/http-header-and-message-body-separator-clarification
    if (_content_length >= 100)
        response_headers = ss.str();
    
    if (!_body.empty())                 { ss << _body; }
    // Convert
    if (_content_length < 100)
        response_headers = ss.str();
    _to_send = ss.str();
    LOG_WRT(Logger::DEBUG, "_to_send = \n" + response_headers + "--------------------\n");
    return (1);
}

/*
** public class methods
*/

int Response::format_to_send(Request *req)
{
    // Status Line
    _http_version = "HTTP/1.1";
    _reason_phrase = code_to_reason[_status_code];
    _content_length = _body.length(); // https://stackoverflow.com/questions/13821263/should-newline-be-included-in-http-response-content-length
	
	// Response headers, dans l'ordre du sujet
    _date = get_date();
    // LOG_WRT(Logger::DEBUG, "format_to_send: " + _date);

    _server = g_conf._webserv;
	if ((req->_method == "GET" || req->_method == "HEAD" || req->_method == "PUT" || req->_method == "POST")
    && (_status_code == OK_200 || _status_code == CREATED_201) && _content_type[0] == "")
		 _content_type[0] = get_content_type(req->_file);
	if (_status_code == CREATED_201)
		_location = get_location_header(req);	
	_content_language.clear();
    _content_location.clear();
    _transfer_encoding.clear();
    _www_authenticate.clear();
	// Response body
	if (req->_method == "HEAD")
		_body.clear();
    concat_to_send();
	return (1);
}

void		Response::handle_response(Request *req)
{
	if (service_unavailable(req)) // 503
        return ;
    else if (bad_request(req)) // 400
		return ;
	else if (method_not_allowed(req)) // 405
		return ;
    else if (unauthorized(req)) // 401
        return ;
    else if (request_entity_too_large(req)) // 413
        return ;
	else if (req->_method == "GET" || req->_method == "HEAD")
		get(req);
	else if (req->_method == "POST")
		post(req);
	else if (req->_method == "PUT")
		put(req);
	else if (req->_method == "DELETE")
		ft_delete(req);
	else if (req->_method == "OPTIONS")
		option(req);
	else if (req->_method == "TRACE")
		trace(req);
	else if (req->_method == "CONNECT")
		connect(req);
}

int		Response::accepted_method(Request *req)
{
    return (req->_method == "GET"
         || req->_method == "HEAD"
         || req->_method == "PUT"
         || req->_method == "POST"
         || req->_method == "DELETE"
         || req->_method == "OPTIONS"
		 || req->_method == "TRACE"
		 || req->_method == "CONNECT");
}

int				Response::bad_request(Request *req)
{
    if (accepted_method(req))
        return (0);
    LOG_WRT(Logger::DEBUG, "BAD_REQUEST_400\n");
    _status_code = BAD_REQUEST_400;
	_allow = vector_to_string(req->_location->_method, ',');
	std::string path = std::string(_client->_server->_error + "/400.html");
    std::ifstream error400(path);
    std::string buffer((std::istreambuf_iterator<char>(error400)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}

int				Response::method_not_allowed(Request *req)
{
	for (std::size_t i = 0; i < (req->_location->_method).size(); ++i)
	{
		LOG_WRT(Logger::DEBUG, "test if " + (req->_location->_method)[i] + " == " + req->_method);
		if ((req->_location->_method)[i] == req->_method)
			return (0);
	}
	LOG_WRT(Logger::DEBUG, "METHOD_NOT_ALLOWED_405\n");
    _status_code = METHOD_NOT_ALLOWED_405;
	_allow = vector_to_string(req->_location->_method, ',');
	std::string path = std::string(_client->_server->_error + "/405.html");
    std::ifstream error405(path);
    std::string buffer((std::istreambuf_iterator<char>(error405)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}

// https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

int				Response::unauthorized(Request *req)
{
    if (!req->_location->_auth.empty())
    {
     	LOG_WRT(Logger::DEBUG, "inside unauthorized():");
     	LOG_WRT(Logger::DEBUG, "1) req->_location->_auth = " + req->_location->_auth);
     	
        // LOG_WRT(Logger::DEBUG, "req->_authorization = " + req->_authorization);
        std::vector<std::string> tokens;
        tokens = utils_tmp::split(req->_authorization, ' ');
        std::string creds = tokens[1];
     	// LOG_WRT(Logger::DEBUG, "creds = " + creds);
     	
        LOG_WRT(Logger::DEBUG, "2) base64_decode(creds) = " + base64_decode(creds));
        if (req->_location->_auth == base64_decode(creds))
        {
            LOG_WRT(Logger::INFO, "Response::unauthorized() ? -> ok authorized\n");
            _status_code = OK_200;
            return (0);
        }
        else
	    {
            LOG_WRT(Logger::INFO, "Response::unauthorized() ? -> ko unauthorized\n");
            _status_code = UNAUTHORIZED_401;
            std::string path = std::string(_client->_server->_error + "/401.html");
            std::ifstream error401(path);
            std::string buffer((std::istreambuf_iterator<char>(error401)), std::istreambuf_iterator<char>());
            _body = buffer;
            return (1);
        }
    }
    else
	    return (0);
}

int				Response::service_unavailable(Request *req)
{
    if (_retry_after == UNAVAILABLE_TIME)
    {
        LOG_WRT(Logger::INFO, "Response::service_unavailable()\n");
        _status_code = SERVICE_UNAVAILABLE_503;
        std::string path = std::string(_client->_server->_error + "/503.html");
        std::ifstream error503(path);
        std::string buffer((std::istreambuf_iterator<char>(error503)), std::istreambuf_iterator<char>());
        _body = buffer;
        return (1);
    }    
    else
        return (0);
}

int				Response::request_entity_too_large(Request *req)
{
    if (req->_saved_error == REQUEST_ENTITY_TOO_LARGE_413
        || (req->_content_length > 0 && req->_location->_max_body > 0))
    {
        if (req->_saved_error == REQUEST_ENTITY_TOO_LARGE_413
            || (req->_content_length > req->_location->_max_body))
        {
            LOG_WRT(Logger::INFO, "Response::request_entity_too_large()\n");
            _status_code = REQUEST_ENTITY_TOO_LARGE_413;
            std::string path = std::string(_client->_server->_error + "/413.html");
            std::ifstream error413(path);
            std::string buffer((std::istreambuf_iterator<char>(error413)), std::istreambuf_iterator<char>());
            _body = buffer;
            return (1);
        }
    }
    return (0);
}

int				Response::not_found(Request *req)
{
	(void)req;
    _status_code = NOT_FOUND_404;
	std::string path = std::string(_client->_server->_error + "/404.html");
    std::ifstream error404(path);
    std::string buffer((std::istreambuf_iterator<char>(error404)), std::istreambuf_iterator<char>());
    _body = buffer;
	return (1);
}

/*
** Reset before waiting for new call
*/

void Response::reset(void)
{
    init();
}
