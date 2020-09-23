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

int	utils_tmp::read_file(int fd, std::string file, std::string &buff)
{
	(void)fd;

	std::fstream newfile;

	newfile.open(file, std::ios::in); //open a file to perform read operation using file object
	if (newfile.is_open())
	{ //checking whether the file is open
		std::string tp;
		while (getline(newfile, tp))
		{					   //read data from file object and put it into string.
			buff += tp + "\n"; //print the data of the string
		}
		newfile.close(); //close the file object.
	}

	/*

	if ((fd = open(file.c_str(), O_RDONLY|O_NONBLOCK, 0666)) == -1)//mettre dans read file
		return (-1);
		int i = 0;
	while ((ret = get_next_line(fd, &cline)))
	{
		std::cout << "line:" << i << std::endl;
		++i;
		line = cline;
		free(cline);
		cline=NULL;
		buff += line + "\n";
	}
	if (cline)
	{
		line = cline;
		free(cline);
		buff += line;
	}
	if (ret < 0)
		return (-1);*/
	return (0);
}

std::string utils_tmp::extract_body(std::string &buff)
{

	//size_t pos = buff.find("\r\n\r\n");
	//buff.erase(0, pos + 4);

	std::string res;
	res = buff.erase(0, (buff.find("\r\n\r\n") + 4));
	return (buff);
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
	//std::cout << "dec_val="<<dec_val << " hexVal"<<hexVal<<std::endl;
    return (dec_val); 
} 


void utils_tmp::remove_return(std::string &str)
{
    size_t pos = str.find_last_of('\r');

    if (pos != std::string::npos)
        str.erase(pos);
}