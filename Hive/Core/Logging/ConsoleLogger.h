#pragma once

#include "Core/Logging/LogManager.h"
namespace hive
{
    struct LogEntry;
    class LogManager;


    class HIVE_API ConsoleLogger : public ILogger
    {
    public:
        ConsoleLogger() = default;
        ~ConsoleLogger() = default;

        void Log(const LogEntry &entry) const override;

    };
}
