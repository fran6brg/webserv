#include "../../includes/Headers.hpp"

void			Response::option(Request *req)
{
	std::string buffer;

	for (std::size_t i = 0; i < (req->_location->_method).size(); ++i)
	{
		buffer = buffer + (req->_location->_method)[i];
		if (i != (req->_location->_method).size() - 1)
			buffer = buffer + ", ";
	}
	_status_code = OK_200;
	_allow = buffer;
}