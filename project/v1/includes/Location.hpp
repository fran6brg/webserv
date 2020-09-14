#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Conf.hpp"

std::vector<std::string> split(const std::string& str, char delim);

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
        
        Location(std::string uri, std::string root, std::string index, std::vector<std::string> method, std::sting php_root, std::string cgi_root)
        {
            _uri = uri;
            _root = root;
            _index = index;
            _method = method;
			_cgi_root = php_cgi; // TEMPORAIRE 
			_php_root = cgi_root; // TEMPORAIRE 
        };
    };

#endif
