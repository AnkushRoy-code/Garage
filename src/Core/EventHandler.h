#ifndef INCLUDE_CORE_EVENTHANDLER_H_
#define INCLUDE_CORE_EVENTHANDLER_H_

#include "Common/pch.h"

namespace Core
{

enum KEY
{
    RESIZE_PROJECT_WINDOW,
    RIGHT,
    LEFT,
    UP,
    DOWN,
    A,
    D,
    COUNT
};

enum KEY_STATE
{
    IDLE,
    PRESSED,
    HELD,
    RELEASED
};

/**
 *   @mermaid{eventHandler}
 */
class InputHandler
{
  public:
    std::array<KEY_STATE, Core::KEY::COUNT> Keys {};

    InputHandler();
    void updateKey(KEY action, bool pressed);
    void endFrame();
};

namespace EventHandler
{
SDL_AppResult handleEvents(SDL_Event *event, InputHandler &inputHandler);
}

}  // namespace Core

#endif  // INCLUDE_CORE_EVENTHANDLER_H_
