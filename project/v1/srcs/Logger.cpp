#include "../includes/Logger.hpp"

Logger Logger::instance;

Logger::Logger() : active(false)
{
}

Logger::~Logger()
{
	instance.Stop();
}

const std::string Logger::PRIORITY_NAMES[] =
{
    "DEBUG",
    "INFO",
    "ERROR"
};
 
void Logger::Start(Priority minPriority, const std::string &logFile, bool date)
{
    instance.active = true;
	instance.isdate = date;
    instance.minPriority = minPriority;
    if (logFile != "")
        instance.fileStream.open(logFile.c_str());
}
 
void Logger::Stop()
{
    instance.active = false;
    if (instance.fileStream.is_open())
        instance.fileStream.close();
}
 
void Logger::Write(Priority priority, const std::string &message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        std::ostream& stream
            = instance.fileStream.is_open() ? instance.fileStream : std::cout;
 
        stream  << PRIORITY_NAMES[priority]
				<< std::setw(7 - (PRIORITY_NAMES[priority]).length());
		if (instance.isdate)
		{
			stream  << " ["
			<< utils_tmp::get_date()
			<< "]";
		}
		stream  << ": "
                << message
                << std::endl;
    }
}

int Logger::Error(const std::string &message)//errno bool
{
	if (errno != 0)
		Logger::Write(Logger::ERROR, message + " -> (" + std::string(strerror(errno)) + ")");
	else
		Logger::Write(Logger::ERROR, message);

	return (EXIT_FAILURE);
}
