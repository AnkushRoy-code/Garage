#ifndef INCLUDE_CORE_EVENTHANDLER_H_
#define INCLUDE_CORE_EVENTHANDLER_H_

#include "SDL3/SDL.h"

namespace EventHandler
{
SDL_AppResult handleEvents(SDL_Event *event);
}

#endif  // INCLUDE_CORE_EVENTHANDLER_H_
