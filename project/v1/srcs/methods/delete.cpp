#include "../../includes/Headers.hpp"

void			Response::ft_delete(Request *req)
{
	int	ret;
	std::ifstream file(req->_file);
	if (file.good())
	{
		ret = remove(req->_file.c_str());
		if (!ret)
			_status_code = OK_200;
		else
			_status_code = ACCEPTED_202;
	}
	else
		_status_code = NO_CONTENT_204;
}