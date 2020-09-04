#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Conf.hpp"

std::vector<std::string> split(const std::string& str, char delim);

class Location
     {
         private:
            Location();
         public:
            Location(std::string uri, std::string root, std::string index, std::string method)
            {
                _uri = uri;
                _root = root;
                _index = index;
                _method = split(method, ',');
            };

            std::string                 _uri;
            std::string                 _root;
            std::string                 _index;
            std::vector<std::string>    _method;
     };

#endif
