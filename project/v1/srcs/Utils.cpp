#include "../includes/Utils.hpp"

std::string get_last_modif(std::string &file)
{
// ------------ > Probleme avec structure "Stat" a la compilation ?
	(void)file;
//	struct stat st;
//	std::string sec;
//	struct tm time;
	std::string date;
//	char buffer[1000];

//	stat(file.c_str(), &st);
//	sec = std::to_string(st.st_mtime).c_str();
//	strptime(sec.c_str(), "%s", &time);
//	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
//	date = buffer;
	return (date);
}

std::string get_date(void)
{
    struct timeval tv;
    struct tm time;
    struct timezone tz;
    char buffer[1000];
    std::string date;

    gettimeofday(&tv, &tz);
    strptime(std::to_string(tv.tv_sec).c_str(), "%s", &time);
    strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", &time);
    date = buffer;
	return (date);
}
