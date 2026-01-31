#pragma once

#include <chrono>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class Logger
{
public:
    static void Init(bool enableConsole = true, bool useColors = true, bool jsonMode = false)
    {
        Logger& inst = Get();
        std::lock_guard<std::mutex> lock(inst.m_Mutex);
        inst.m_EnableConsole = enableConsole;
        inst.m_UseColors = useColors;
        inst.m_JsonMode = jsonMode;

        if (!inst.m_WorkerStarted)
        {
            inst.m_Exit = false;
            inst.m_WorkerThread = std::thread(&Logger::WorkerLoop, &inst);
            inst.m_WorkerStarted = true;
        }
    }

    static void RegisterSubsystemFile(const std::string& subsystem, const std::string& filePath, bool append = true)
    {
        Logger& inst = Get();
        std::lock_guard<std::mutex> lock(inst.m_Mutex);

        EnsureDirectoryExists(filePath);

        FileInfo info;
        info.basePath = std::filesystem::path(filePath);
        info.rotationIndex = 0;

        std::ios::openmode mode = std::ios::out;
        if (append)
            mode |= std::ios::app;

        info.stream.open(info.basePath, mode);
        if (!info.stream.is_open())
        {
            std::cerr << "[LOGGER] Failed to open log file for subsystem '" << subsystem << "': " << filePath
                      << std::endl;
            return;
        }

        inst.m_FileStreams[subsystem] = std::move(info);
    }

    static void Shutdown()
    {
        Logger& inst = Get();

        {
            std::lock_guard<std::mutex> lock(inst.m_Mutex);
            inst.m_Exit = true;
        }
        inst.m_Cv.notify_all();

        if (inst.m_WorkerStarted && inst.m_WorkerThread.joinable())
            inst.m_WorkerThread.join();

        std::lock_guard<std::mutex> lock(inst.m_Mutex);
        for (auto& [subsystem, info] : inst.m_FileStreams)
        {
            if (info.stream.is_open())
                info.stream.close();
        }
        inst.m_FileStreams.clear();
    }

    static void Log(LogLevel level, const std::string& msg, const std::string& subsystem = "Core")
    {
#ifdef DEBUG
        Logger& inst = Get();

        {
            std::lock_guard<std::mutex> lock(inst.m_Mutex);
            inst.m_Queue.push_back(LogMessage{level, msg, subsystem});
        }

        inst.m_Cv.notify_one();
#else
        (void)level;
        (void)msg;
        (void)subsystem;
#endif
    }

private:
    Logger() = default;
    ~Logger() = default;

    struct LogMessage
    {
        LogLevel level;
        std::string message;
        std::string subsystem;
    };

    struct FileInfo
    {
        std::ofstream stream;
        std::filesystem::path basePath;
        std::size_t rotationIndex = 0;
    };

    static Logger& Get()
    {
        static Logger instance;
        return instance;
    }

    void WorkerLoop()
    {
        while (true)
        {
            LogMessage entry;

            {
                std::unique_lock<std::mutex> lock(m_Mutex);
                m_Cv.wait(lock, [&] { return m_Exit || !m_Queue.empty(); });

                if (m_Exit && m_Queue.empty())
                    break;

                entry = std::move(m_Queue.front());
                m_Queue.pop_front();
            }

            ProcessEntry(entry);
        }
    }

    void ProcessEntry(const LogMessage& entry)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        std::string line = m_JsonMode ? FormatJsonLine(entry.level, entry.message, entry.subsystem)
                                      : FormatTextLine(entry.level, entry.message, entry.subsystem);

        if (m_EnableConsole)
        {
            if (m_UseColors && !m_JsonMode)
            {
                const char* color = ColorForLevel(entry.level);
                std::cout << color << line << "\033[0m" << std::endl;
            }
            else
            {
                std::cout << line << std::endl;
            }
        }

        WriteToSubsystemFile(entry.subsystem, line);

        if (entry.subsystem != "Global")
            WriteToSubsystemFile("Global", line);
    }

    void WriteToSubsystemFile(const std::string& subsystem, const std::string& line)
    {
        auto it = m_FileStreams.find(subsystem);
        if (it == m_FileStreams.end())
            return;

        FileInfo& info = it->second;

        if (!info.stream.is_open())
            return;

        constexpr std::uintmax_t MAX_SIZE = 5ull * 1024ull * 1024ull; // 5 MB

        try
        {
            if (std::filesystem::exists(info.basePath))
            {
                auto size = std::filesystem::file_size(info.basePath);
                if (size >= MAX_SIZE)
                {
                    RotateFile(info);
                }
            }
        }
        catch (...)
        {
        }

        info.stream << line << std::endl;
    }

    void RotateFile(FileInfo& info)
    {
        if (info.stream.is_open())
            info.stream.close();

        info.rotationIndex++;

        std::filesystem::path base = info.basePath;
        std::filesystem::path parent = base.parent_path();
        std::string stem = base.stem().string();
        std::string extension = base.extension().string();

        std::string newName = stem + "_" + std::to_string(info.rotationIndex) + extension;
        std::filesystem::path newPath = parent / newName;

        info.basePath = newPath;

        std::ios::openmode mode = std::ios::out;
        info.stream.open(info.basePath, mode);
        if (!info.stream.is_open())
        {
            std::cerr << "[LOGGER] Failed to rotate log file to " << info.basePath.string() << std::endl;
        }
    }

    std::string FormatTextLine(LogLevel level, const std::string& msg, const std::string& subsystem)
    {
        std::ostringstream oss;
        oss << "[" << MakeTimestamp() << "]"
            << " [T:" << ThreadIdString() << "]"
            << " [" << subsystem << "]"
            << " [" << LevelToString(level) << "] " << msg;
        return oss.str();
    }

    std::string FormatJsonLine(LogLevel level, const std::string& msg, const std::string& subsystem)
    {
        std::ostringstream oss;
        oss << "{"
            << "\"timestamp\":\"" << EscapeJson(MakeTimestamp()) << "\","
            << "\"thread\":\"" << EscapeJson(ThreadIdString()) << "\","
            << "\"subsystem\":\"" << EscapeJson(subsystem) << "\","
            << "\"level\":\"" << EscapeJson(LevelToString(level)) << "\","
            << "\"message\":\"" << EscapeJson(msg) << "\""
            << "}";
        return oss.str();
    }

    static std::string MakeTimestamp()
    {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto t = system_clock::to_time_t(now);
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

    static std::string ThreadIdString()
    {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return oss.str();
    }

    static std::string EscapeJson(const std::string& s)
    {
        std::ostringstream oss;
        for (char c : s)
        {
            switch (c)
            {
                case '\"':
                    oss << "\\\"";
                    break;
                case '\\':
                    oss << "\\\\";
                    break;
                case '\b':
                    oss << "\\b";
                    break;
                case '\f':
                    oss << "\\f";
                    break;
                case '\n':
                    oss << "\\n";
                    break;
                case '\r':
                    oss << "\\r";
                    break;
                case '\t':
                    oss << "\\t";
                    break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20)
                        oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    else
                        oss << c;
            }
        }
        return oss.str();
    }

    const char* ColorForLevel(LogLevel level) const
    {
        switch (level)
        {
            case LogLevel::Info:
                return "\033[37m"; // gris
            case LogLevel::Warning:
                return "\033[33m"; // jaune
            case LogLevel::Error:
                return "\033[31m"; // rouge
            default:
                return "\033[37m";
        }
    }

    const char* LevelToString(LogLevel level) const
    {
        switch (level)
        {
            case LogLevel::Info:
                return "INFO";
            case LogLevel::Warning:
                return "WARN";
            case LogLevel::Error:
                return "ERROR";
            default:
                return "UNK";
        }
    }

    static void EnsureDirectoryExists(const std::string& filePath)
    {
        std::filesystem::path p(filePath);
        auto parent = p.parent_path();
        if (!parent.empty() && !std::filesystem::exists(parent))
        {
            std::filesystem::create_directories(parent);
        }
    }

private:
    bool m_EnableConsole = true;
    bool m_UseColors = true;
    bool m_JsonMode = false;

    std::unordered_map<std::string, FileInfo> m_FileStreams;

    std::deque<LogMessage> m_Queue;
    std::mutex m_Mutex;
    std::condition_variable m_Cv;
    std::thread m_WorkerThread;
    bool m_WorkerStarted = false;
    bool m_Exit = false;
};
