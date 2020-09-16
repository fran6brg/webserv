#include "../../includes/Headers.hpp"

void			Response::get(Request *req)
{
	int		language = 0;
	int		charset = 0;

	language = set_laguage(req);
	charset = set_charset(req);

//	std::cout << req->_file << std::endl;
//	std::cout << "language = " << language << std::endl;
	std::ifstream file(req->_file);
	if (req->_method == "GET" && (req->_location->_cgi_root != "" || (req->_location->_php_root != "" && is_php(req->_file))))
	{
		ft_cgi(req);
		get_cgi_ret();
		std::ifstream temp("./www/temp_file");
		std::string buffer((std::istreambuf_iterator<char>(temp)), std::istreambuf_iterator<char>());

		_body = buffer;
		remove("./www/temp_file");
	}
	else if (file.good())
	{
		std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		_body = buffer;
		_last_modified = get_last_modif(req->_file);
		_status_code = OK_200;
	}
	else
		not_found(req);
//	std::cout << req->_file << std::endl;
	if (charset)
		unset_extension(req);
	if (language)
		unset_extension(req);
}
