#include "Console.h"
#include <chrono>
#include <sstream>
#include <iomanip>

int ConsoleLogBuffer::index = 0;
std::vector<ConsoleLogMessage> ConsoleLogBuffer::ConsoleLogs {};

void ConsoleLogBuffer::addMessage(const std::string &msg)
{
    std::string timestamp      = getCurrentTime();
    ConsoleLogMessage logEntry = {msg, timestamp};
    {
        if (ConsoleLogs.size() < MAX_MESSAGES) { ConsoleLogs.push_back(logEntry); }
        else
        {
            ConsoleLogs[index] = logEntry;  // Overwrite oldest
        }
        index = (index + 1) % MAX_MESSAGES;
    }
}

const std::vector<ConsoleLogMessage> &ConsoleLogBuffer::getMessages()
{
    return ConsoleLogs;
}

std::string ConsoleLogBuffer::getCurrentTime()
{
    auto now       = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf {};
    localtime_r(&in_time_t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    return oss.str();
}
