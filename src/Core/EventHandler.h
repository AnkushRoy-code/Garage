#ifndef INCLUDE_CORE_EVENTHANDLER_H_
#define INCLUDE_CORE_EVENTHANDLER_H_

#include "Common/pch.h"

namespace Core
{

enum KEY
{
    // window
    RESIZE_PROJECT_WINDOW,

    // mouse
    MOUSE_ROLL,
    MOUSE_MIDDLE_CLICK,
    MOUSE_RIGHT_CLICK,
    MOUSE_LEFT_CLICK,
    MOUSE_MOVE,

    // keyboard keys
    ESC,
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
class EventHandlerStruct
{
  public:
    std::array<KEY_STATE, Core::KEY::COUNT> Keys {};

    EventHandlerStruct();
    bool GetEventHeld(KEY p_Key);
    bool GetEventPressed(KEY p_Key);
    void UpdateKey(KEY p_Action, bool p_Pressed);
    void EndFrame();
    SDL_AppResult HandleEvents(SDL_Event *p_Event);
};

}  // namespace Core

#endif  // INCLUDE_CORE_EVENTHANDLER_H_
