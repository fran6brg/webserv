#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Conf.hpp"

// std::vector<std::string> split(const std::string &str, char delim);

class Location
    {
        private:
        Location();
        public:
        
        std::string                 _uri;
        std::string                 _root;
        std::string                 _index;
        std::vector<std::string>    _method;
		std::string					_cgi_root;
		std::string					_php_root;
		std::string					_cgi;
		int							_autoindex;
		int							_max_body;
		std::string					_auth;
        
        Location(std::string uri, std::string root, std::string index, std::vector<std::string> method,
				std::string cgi_root, std::string php_root, std::string cgi, int autoindex, int max_body,
                std::string auth)
        {
            _uri = uri;
            _root = root;
            _index = index;
            _method = method;
			_cgi_root = cgi_root;
			_php_root = php_root;
			_cgi = cgi;
			_autoindex = autoindex;
			_max_body = max_body;
			_auth = auth;
        };
    };

#endif
