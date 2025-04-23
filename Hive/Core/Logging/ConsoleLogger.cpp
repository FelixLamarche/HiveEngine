#include "ConsoleLogger.h"
#include "LogManager.h"

#include <iostream>

void hive::ConsoleLogger::Log(const LogEntry& entry) const
{
    std::string prefix;
    switch (entry.level)
    {
    case LOG_LEVEL_DEBUG:
        prefix = "[DEBUG]";
        break;
    case LOG_LEVEL_INFO:
        prefix = "[INFO]";
        break;
    case LOG_LEVEL_WARN:
        prefix = "[WARN]";
        break;
    case LOG_LEVEL_ERROR:
        prefix = "[ERROR]";
        break;
    }
    std::cout << entry.timestamp << " " + prefix + " " << entry.message << std::endl;
}
