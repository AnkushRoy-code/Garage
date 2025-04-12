#ifndef INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
#define INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_

#include "Core/ImGuiCore/ScrollingBuffer.h"

class Tracker
{
  public:
    static void AddUpdateFPSPointQueue(float UpdateTime);
    static void AddRenderFPSPointQueue(float RenderTime);

    static void RegisterPoints();
    static ScrollingBuffer s_UpdateFPSBuffer;
    static ScrollingBuffer s_RealUpdateFPSBuffer;
    static ScrollingBuffer s_RenderFPSBuffer;

  private:
    static float m_UpdateTime;
    static float m_RenderTime;
};

#endif  // INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
