#include "../includes/Headers.hpp"

std::string 	get_last_modif(std::string file)
{
	struct stat 	info;
	std::string 	sec;
	struct tm 		time;
	std::string 	date;
	char 			buffer[1000];

	stat(file.c_str(), &info);
	sec = std::to_string(info.st_mtime).c_str();
	strptime(sec.c_str(), "%s", &time);
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S CEST", &time);
	date = buffer;
	return (date);
}

std::string 	get_date(void)
{
    struct timeval 	tv;
    struct tm 		time;
    struct timezone tz;
    char 			buffer[1000];
    std::string 	date;

    gettimeofday(&tv, &tz);
    strptime(std::to_string(tv.tv_sec).c_str(), "%s", &time);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S CEST", &time);
    date = buffer;
	return (date);
}

std::string		get_content_type(std::string file)
{
	std::string		temp;
	int	j = 0;
	int	i = file.size() - 1;
	std::string ext[67] = {"php", "aac", "abw", "arc", "avi", "azw", "bin", "bz", "bz2", "csh", "css", "csv", "doc", "docsx", "eot", "epub", "gif", "htm", "html", "ico",
	"ics", "jar", "jpeg", "jpg", "js", "json", "mid", "midi", "mpeg", "mpkg", "odp", "ods", "odt", "oga", "ogv", "ogx", "otf", "png", "pdf", "ppt", "pptx", "rar", "rtf", "sh"
	"svg", "swf", "tar", "tif", "tiff", "ts", "ttf", "vsd", "wav", "weba", "webm", "webp", "woff" ,"woff2", "xhtml", "xls", "xlsx","xml", "xul", "zip", "3gp", "3g2", "7z"};
	std::string ret[67] = {"text/html", "audio/aac", "application/x-abiword", "application/octet-stream", "video/x-msvideo", "application/vnd.amazon.ebook", "application/octet-stream", "application/x-bzip",
	"application/x-bzip2", "application/x-csh", "text/css", "text/csv", "application/msword", "application/vnd.openxmlformats-officedocument.wordprocessingml.document", "application/vnd.ms-fontobject",
	"application/epub+zip", "image/gif", "text/html", "text/html", "image/x-icon", "text/calendar", "application/java-archive", "image/jpeg", "image/jpeg" ,"application/javascript", "application/json",
	"audio/midi","audio/midi", "video/mpeg", "	application/vnd.apple.installer+xml", "application/vnd.oasis.opendocument.presentation", "application/vnd.oasis.opendocument.spreadsheet", "application/vnd.oasis.opendocument.text", "audio/ogg", "video/ogg", "application/ogg", "font/otf", "image/png", "application/pdf", "application/vnd.ms-powerpoint", "application/vnd.openxmlformats-officedocument.presentationml.presentation",
	"application/x-rar-compressed", "application/rtf", "application/x-sh", "image/svg+xml", "application/x-shockwave-flash", "application/x-tar", "image/tiff", "image/tiff", "application/typescript", "font/ttf", "application/vnd.visio", "audio/x-wav", "audio/webm", "video/webm", "image/webp", "font/woff", "font/woff2", "application/xhtml+xml", "application/vnd.ms-excel", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
	"application/xml", "application/vnd.mozilla.xul+xml", "application/zip", "video/3gpp", "video/3gpp2", "application/x-7z-compressed"};
	
	while (file[i] != '.')
		i--;
	temp = file.substr(i + 1, file.size());
	while (j < 67)
	{
		if (ext[j] == temp)
			return (ret[j]);
		j++;
	}
	return ("text/plain");
}

std::string		get_location_header(Request *req)
{
	int				i = req->_file.size() - 1;
	int				j = req->_location->_uri.size() - 1;
	std::string		temp;
	while (req->_file[i] != '/')
		i--;
	if (req->_location->_uri[j] == '/')
		temp = req->_location->_uri + req->_file.substr(i + 1, req->_file.size());
	else
		temp = req->_location->_uri + "/" + req->_file.substr(i + 1, req->_file.size());
	return (temp);
}

std::string map_to_string(std::map<int, std::string> map, char delim)
{
    std::stringstream ret;
    size_t i = 0;

    ret.clear();    
    while (i < map.size())
    {
        ret << map[int(i)];
        if ((i + 1) < map.size())
            ret << delim << " ";
        ++i;
    }
    return (ret.str());
}

void displayMap(std::map<std::string, std::string> map)
{
	std::map<std::string, std::string>::iterator it = map.begin();
	int i = 0;
	while (it != map.end())
	{
		std::cout << ++i << ") " << it->first << "=" << it->second << std::endl;
		++it;
	}
}