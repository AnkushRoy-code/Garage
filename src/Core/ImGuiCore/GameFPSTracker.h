#ifndef INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
#define INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_

#include "ScrollingBuffer.h"

class Tracker
{
  public:
    static void AddUpdateFPSPointQueue(float UpdateTime);
    static void AddRenderFPSPointQueue(float RenderTime);

    static void registerPoints();
    static ScrollingBuffer UpdateFPSBuffer;
    static ScrollingBuffer RealUpdateFPSBuffer;
    static ScrollingBuffer RenderFPSBuffer;

  private:
    static float updateTime;
    static float renderTime;
};

#endif  // INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
