#ifndef INCLUDE_CORE_CONSOLE_CPP_
#define INCLUDE_CORE_CONSOLE_CPP_

#include "Common/pch.h"

namespace Core
{

struct ConsoleLogMessage
{
    std::string message;
    std::string timestamp;
};

class ConsoleLogBuffer
{
  public:
    static void AddMessage(const std::string &p_Message);
    static const std::vector<ConsoleLogMessage> &GetMessages();

  private:
    static int m_Index;
    static std::vector<ConsoleLogMessage> m_ConsoleLogs;
    static const size_t k_MaxMessages = 100;

  private:
    static std::string GetCurrentTime();
};

}  // namespace Core
#endif  // INCLUDE_CORE_CONSOLE_CPP_
