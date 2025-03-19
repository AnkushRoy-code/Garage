#include "SimpleExample.h"
#include "Common/Common.h"
#include "Core/Console.h"

bool SimpleExample::Init()
{
    Name = "SimpleExample";
    hasUI = false;
    ConsoleLogBuffer::addMessage("SimpleExample Initialised\n");
    return true;
}
bool SimpleExample::Update()
{
    return true;
}

bool SimpleExample::Draw()
{
    PROFILE_SCOPE_N("SimpleExample::Draw");

    return true;
}

void SimpleExample::Quit() {}
