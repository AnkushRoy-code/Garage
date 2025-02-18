#include "../../Common/Context.h"
#include <print>

namespace SimpleExample2
{

static bool Init(Context &context)
{
    std::println("SimpleExample2 Initialised");
    return true;
}

static bool Update(Context &context)
{
    return true;
}

static bool Draw(Context &context)
{
    return true;
}

static void Quit(Context &context)
{
    std::println("Quit SimpleExample2");
}

}  // namespace SimpleExample

bool registerSimpleExample2();
