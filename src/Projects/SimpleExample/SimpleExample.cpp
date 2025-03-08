#include "SimpleExample.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"
#include <iostream>

bool SimpleExample::Init(Context &context)
{
    std::cout << "SimpleExample Initialised\n";
    return true;
}
bool SimpleExample::Update(Context &context) 
{
    return true;
}

bool SimpleExample::Draw(Context &context) 
{
    PROFILE_SCOPE_N("SimpleExample::Draw");
    return true;
}

void SimpleExample::Quit(Context &context)  {}
