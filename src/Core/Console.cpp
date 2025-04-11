#include "Console.h"

namespace Core
{

int ConsoleLogBuffer::m_Index = 0;
std::vector<ConsoleLogMessage> ConsoleLogBuffer::m_ConsoleLogs {};

void ConsoleLogBuffer::AddMessage(const std::string &p_Message)
{
    const std::string timestamp      = GetCurrentTime();
    ConsoleLogMessage logEntry = {p_Message, timestamp};
    {
        if (m_ConsoleLogs.size() < k_MaxMessages) { m_ConsoleLogs.push_back(logEntry); }
        else
        {
            m_ConsoleLogs[m_Index] = logEntry;  // Overwrite oldest
        }
        m_Index = (m_Index + 1) % k_MaxMessages;
    }
}

const std::vector<ConsoleLogMessage> &ConsoleLogBuffer::GetMessages()
{
    return m_ConsoleLogs;
}

std::string ConsoleLogBuffer::GetCurrentTime()
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
