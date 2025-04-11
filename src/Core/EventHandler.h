#ifndef INCLUDE_CORE_EVENTHANDLER_H_
#define INCLUDE_CORE_EVENTHANDLER_H_

#include "Common/pch.h"

namespace Core
{

enum KEY
{
    RESIZE_PROJECT_WINDOW,
    MOUSE_ROLL,
    MOUSE_MIDDLE_CLICK,
    MOUSE_RIGHT_CLICK,
    MOUSE_LEFT_CLICK,
    RIGHT,
    LEFT,
    UP,
    DOWN,
    W,
    A,
    S,
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
    bool getEventHeld(KEY key);
    bool getEventPressed(KEY key);
    void updateKey(KEY action, bool pressed);
    void endFrame();
};

namespace EventHandler
{
SDL_AppResult handleEvents(SDL_Event *event, InputHandler &inputHandler);
}

}  // namespace Core

#endif  // INCLUDE_CORE_EVENTHANDLER_H_
