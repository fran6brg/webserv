#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <string>
# include <fstream>
# include <iostream>
# include <iomanip>

# include "utils_tmp.hpp"

# define LOG_START(MIN_PRIORITY, FILE, DATE) Logger::Start(MIN_PRIORITY, FILE, DATE);
# define LOG_STOP() Logger::Stop();
# define LOG_WRT(PRIORITY, MESSAGE) Logger::Write(PRIORITY, MESSAGE);
# define ERROR_RET(MESSAGE) Logger::Error(MESSAGE)
 
class Logger
{
public:
    enum Priority
    {
        DEBUG,
        INFO,
        ERROR,
		CLEAR
    };

    static void Start(Priority minPriority, const std::string &logFile, bool date);
    static void Stop();
    static void Write(Priority priority, const std::string &message);
	static int Error(const std::string &message);
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
