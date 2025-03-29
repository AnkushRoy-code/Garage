#include "Console.h"

#include <chrono>
#include <sstream>
#include <iomanip>

namespace Core
{

int ConsoleLogBuffer::index = 0;
std::vector<ConsoleLogMessage> ConsoleLogBuffer::ConsoleLogs {};

void ConsoleLogBuffer::addMessage(const std::string &msg)
{
    const std::string timestamp      = getCurrentTime();
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
    const auto now       = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf {};
#ifdef _WIN32
    localtime_s(&tm_buf, &in_time_t);
#else
    localtime_r(&in_time_t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    return oss.str();
}

}  // namespace Core
