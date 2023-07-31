#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// Create a class called log that's a singleton and uses constexpr for log levels
class log
{
public:
    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    static log &get()
    {
        static log instance;
        return instance;
    }

    static void output(Level level, const char *format, va_list args)
    {
        auto instance = log::get();
        if (level >= instance.m_level)
        {
            const auto now = time(nullptr);
            const auto time = localtime(&now);
            fprintf(stdout, "[%d-%02d-%02d %02d:%02d:%02d] - ", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
            fprintf(stdout, instance.buildFormatPrefix(level));
            vfprintf(stdout, format, args);
            fprintf(stdout, "\n");
            va_end(args);
        }
    }

    static inline void debug(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        log::output(Level::Debug, format, args);
        va_end(args);
    }

    static inline void info(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        log::output(Level::Info, format, args);
        va_end(args);
    }

    static inline void warning(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        log::output(Level::Warning, format, args);
        va_end(args);
    }

    static inline void error(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        log::output(Level::Error, format, args);
        va_end(args);
    }

    static inline void fatal(const char *format, ...)
    {
        va_list args;
        va_start(args, format);
        log::output(Level::Fatal, format, args);
        va_end(args);
    }

    static inline void setLevel(Level level)
    {
        auto instance = log::get();
        instance.m_level = level;
    }

private:
    constexpr log() {}
    ~log() {}

    Level m_level = Level::Debug;

    static const char *buildFormatPrefix(Level level)
    {
        switch (level)
        {
        case Level::Debug:
            return "[DEBUG]:";
        case Level::Info:
            return "[INFO]: ";
        case Level::Warning:
            return "[WARN]: ";
        case Level::Error:
            return "[ERROR]: ";
        case Level::Fatal:
            return "[FATAL]: ";
        }
    }
};