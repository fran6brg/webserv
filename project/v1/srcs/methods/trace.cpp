#include "../../includes/Headers.hpp"

void			Response::trace(Request *req)
{
	(void)req;
	int		i;
	if (!_status_code)
	{
    	_http_version = "HTTP/1.1";
    	_server = g_conf._webserv;
		_status_code = OK_200;
    	_reason_phrase = code_to_reason[_status_code];
		_content_type[0] = "message/http";
		_date = get_date();
    	concat_to_send();
		i = _to_send.size() - 3;
		_to_send[i] = '\0';
		_to_send[i + 1] = '\0';
		_to_send[i + 2] = '\0';
		_body = _to_send;
		// Supprimer les retour a la ligne a la fin du header avant de les envoyer dans le body ?
	}
}