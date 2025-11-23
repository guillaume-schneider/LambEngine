#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

enum class LogLevel { Info, Warning, Error };

class Logger
{
public:
    enum class Mode { Console, File };

    static void Init(Mode mode, const std::string& filePath = "engine.log")
    {
        Get().m_Mode = mode;

        if (mode == Mode::File) {
            Get().m_FileStream.open(filePath, std::ios::out | std::ios::app);
            if (!Get().m_FileStream.is_open()) {
                std::cerr << "[ERROR] Failed to open log file: " << filePath << std::endl;
                Get().m_Mode = Mode::Console; // fallback
            }
        }
    }

    static void Shutdown()
    {
        if (Get().m_FileStream.is_open())
            Get().m_FileStream.close();
    }

    static void Log(LogLevel level, const std::string& msg)
    {
#ifdef DEBUG   // enable logs only in debug mode
        std::lock_guard<std::mutex> lock(Get().m_Mutex);

        const char* prefix = "[INFO] ";
        if (level == LogLevel::Warning) prefix = "[WARN] ";
        if (level == LogLevel::Error)   prefix = "[ERROR]";

        if (Get().m_Mode == Mode::Console) {
            std::cout << prefix << " " << msg << std::endl;
        }
        else if (Get().m_Mode == Mode::File) {
            if (Get().m_FileStream.is_open())
                Get().m_FileStream << prefix << " " << msg << std::endl;
        }
#else
        (void)level;
        (void)msg;
#endif
    }

private:
    Logger() = default;

    static Logger& Get()
    {
        static Logger instance;
        return instance;
    }

    Mode m_Mode = Mode::Console;         // default
    std::ofstream m_FileStream;
    std::mutex m_Mutex;                  // thread-safe
};
