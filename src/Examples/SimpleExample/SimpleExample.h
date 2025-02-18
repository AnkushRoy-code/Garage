#include "../../Common/Context.h"
#include <print>

namespace SimpleExample
{

static bool Init(Context &context)
{
    std::println("SimpleExample Initialised");
    return true;
}

static bool Update(Context &context)
{
    // std::println("Update works");
    return true;
}

static bool Draw(Context &context)
{
    // std::println("Drawing works");
    return true;
}

static void Quit(Context &context)
{
    std::println("Quit SimpleExample");
}

}  // namespace SimpleExample

bool registerSimpleExample();
