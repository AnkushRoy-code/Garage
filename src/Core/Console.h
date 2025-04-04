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
    static const size_t MAX_MESSAGES = 100;

    static void addMessage(const std::string &msg);
    static const std::vector<ConsoleLogMessage> &getMessages();

    static std::vector<ConsoleLogMessage> ConsoleLogs;
    static int index;

    static std::string getCurrentTime();
};

}  // namespace Core
#endif  // INCLUDE_CORE_CONSOLE_CPP_
