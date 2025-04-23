#pragma once
#include <Core/Memory/MemoryAllocator.h>
#include <Core/Object/Object.h>

namespace hive
{
    enum LogLevel
    {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
    };

    struct LogEntry
    {
        std::string message;
        std::string timestamp;
        LogLevel level;
    };

    class HIVE_API ILogger : public Object
    {
    public:
        virtual ~ILogger() = default;
        virtual void Log(const LogEntry &entry) const = 0;

    };

    /**
     * @brief Manage the logging system
     * @note The LogManager owns the callbacks... (ILogger)
     */
    class HIVE_API LogManager : public Object
    {
    public:
        void Init();
        void Shutdown();
        using LoggerID = int;


        /**
         *
         * @param logger pointer to an allocated ILogger (will take ownership)
         */
        LoggerID AddLogger(const ILogger *logger);
        void RemoveCallback(LoggerID id);
        void SetLogLevel(LogLevel level);

        void Log(LogLevel level, const char* msg, ...) const;
    private:
        std::unordered_map<LoggerID, const ILogger*> p_callbacks{};
        int m_idCounter = 0;
        LogLevel m_logLevel = LOG_LEVEL_INFO;
    };

    /**
     * @brief Get the global Log manager (Lazy loaded)
     * @return the global logging manager
     */
    HIVE_API LogManager &GetLogManager();


}


#define HIVE_LOG_DEBUG(msg, ...) hive::GetLogManager().Log(hive::LogLevel::LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#define HIVE_LOG_INFO(msg, ...) hive::GetLogManager().Log(hive::LogLevel::LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#define HIVE_LOG_WARN(msg, ...) hive::GetLogManager().Log(hive::LogLevel::LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#define HIVE_LOG_ERROR(msg, ...) hive::GetLogManager().Log(hive::LogLevel::LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)
