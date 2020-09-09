#ifndef LOGGER_HPP
# define LOGGER_HPP


#include "../includes/Headers.hpp" //get_date()

# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>

#define LOGGER_START(MIN_PRIORITY, FILE, DATE) Logger::Start(MIN_PRIORITY, FILE, DATE);
#define LOGGER_STOP() Logger::Stop();
#define LOGGER_WRITE(PRIORITY, MESSAGE) Logger::Write(PRIORITY, MESSAGE);
 
class Logger
{
public:
    enum Priority
    {
        DEBUG,
        INFO,
        ERROR
    };

    static void Start(Priority minPriority, const std::string &logFile, bool date);
    static void Stop();
    static void Write(Priority priority, const std::string &message);
	~Logger();
 
private:
    Logger();
    Logger(const Logger &the_Logger) = delete;
    Logger &operator = (const Logger &the_Logger) = delete;
 
    bool						active;
    bool						isdate;
    std::ofstream				fileStream;
    Priority					minPriority;
    static const std::string	PRIORITY_NAMES[];
    static Logger				instance;
};

#endif
