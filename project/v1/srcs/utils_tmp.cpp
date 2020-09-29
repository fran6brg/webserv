#include "../includes/utils_tmp.hpp"

std::string	utils_tmp::get_date(void)
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

int utils_tmp::isspace(int c)
{
	char space[] = "\t\n\v\f\r ";
	int i = -1;

	while (++i < 6)
		if ((char)c == space[i])
			return (1);
	return (0);
}

std::vector<std::string> utils_tmp::split_string(std::string &str, std::string set)
{
	std::vector<std::string> split;
	std::size_t p1;
	std::size_t p2 = 0;
	
	while (p2 < str.length())
	{
		if ((p1 = str.find_first_not_of(set, p2)) == std::string::npos)
			return (split);
		if ((p2 = str.find_first_of(set, p1)) == std::string::npos)
			p2 = str.length();
		split.push_back(str.substr(p1, (p2 - p1)));
	}
	return (split);
}

bool utils_tmp::file_exists(const char *filename)
{
	struct stat	buffer;
	return (stat (filename, &buffer) == 0);
}

int utils_tmp::get_buffer(std::string file, std::string &buff)
{
    int fd;	
	int ret;	
	char buffer[BUFFER_SIZE + 1];	
	if ((fd = open(file.c_str(), O_RDONLY|O_NONBLOCK)) < 0)	
		return (-1);	
	while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)	
	{	
		buffer[ret] = '\0'; 	
		buff += buffer;	
	}	
	close(fd);	
	return (ret);	
}

int utils_tmp::extract_body(std::string &buff)
{
	size_t pos;
	
	if ((pos = buff.find("\r\n\r\n")) == std::string::npos)
		return (-1);
	buff.erase(0, pos + 4);
		
	return (0);
}

int utils_tmp::hexa_to_dec(const char *hexVal)
{    
    int len = strlen(hexVal); 
    int base = 1; 
    int dec_val = 0; 

	for (int i = len - 1; i >= 0; --i) 
    {    
        if (hexVal[i] >= '0' && hexVal[i] <= '9') 
        { 
            dec_val += (hexVal[i] - 48) * base;
            base = base * 16; 
        } 
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
            base = base * 16; 
        } 
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f') 
        { 
            dec_val += (hexVal[i] - 87)*base; 
            base = base * 16; 
        } 
    }
    return (dec_val); 
} 


void utils_tmp::remove_return(std::string &str)
{
    size_t pos = str.find_last_of('\r');

    if (pos != std::string::npos)
        str.erase(pos);
}

size_t	utils_tmp::getSecondsDiff(std::string complete_time)
{
	struct tm		complete;
	struct tm		*now;
	struct timeval	now_timeval;
	size_t			diff = 0;

	strptime(complete_time.c_str(), "%a, %d %b %Y %T", &complete);
	gettimeofday(&now_timeval, NULL);
	now = localtime(&now_timeval.tv_sec);
	diff = (now->tm_hour - complete.tm_hour) * 3600;
	diff += (now->tm_min - complete.tm_min) * 60;
	diff += (now->tm_sec - complete.tm_sec);
	return (diff);
}

void	utils_tmp::ft_getline(std::string &b, std::string &line)
{
    size_t					pos;

    pos = b.find("\n");

    if (pos != std::string::npos)
    {
        line = std::string (b, 0, pos++);
        b = b.substr(pos);
    }
    else
    {
        if (b[b.size() - 1] == '\n')
            b = b.substr(b.size());
        else
        {
            line = b;
            b = b.substr(b.size());
        }
    }
}

std::vector<std::string> utils_tmp::split(const std::string& str, char delim)
{
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

std::string utils_tmp::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');

    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(' ');
    return (std::string(str.substr(first, (last - first + 1))));
}

void utils_tmp::print_map(std::stringstream &ss1, std::map<int, std::string> map)
{
    size_t i = 0;
    
    if (map.empty())
        ss1 << std::endl;
    else
    {
        while (i < map.size())
            ss1 << " " << map[i++];
        ss1 << std::endl;
    }
}

void utils_tmp::free_strtab(char ***tab)
{
    size_t i = 0;
    while ((*tab)[i])
        i++;
    size_t j = 0;
    while (j <= i)
    {
        free((*tab)[j++]);
    }
    free(*tab);
}

int utils_tmp::is_valide_methods(std::string &meth)
{
	if (meth == "GET"
     || meth == "HEAD"
     || meth == "PUT"
     || meth == "POST"
     || meth == "DELETE"
     || meth == "OPTIONS"
	 || meth == "TRACE"
	 || meth == "CONNECT")
		return (1);
	return (0);
}