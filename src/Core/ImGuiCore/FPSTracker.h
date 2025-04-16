#ifndef INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
#define INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_

#include "Core/ImGuiCore/ScrollingBuffer.h"

class Tracker
{
  public:
    /// This function adds the time taken by the update function to complete in a Queue. Run
    /// RegisterPoints() to actually register the point in the tracker
    static void AddUpdateFPSPointQueue(float UpdateTime);

    /// This function adds the time taken by the render function to complete in a Queue. Run
    /// RegisterPoints() to actually register the point in the tracker
    static void AddRenderFPSPointQueue(float RenderTime);

    /// This function creates new points in the tracker using the values from
    /// AddRenderFPSPointQueue() and AddUpdateFPSPointQueue(). If those function were not used the
    /// value that will be put up in tracker will be 0.
    static void RegisterPoints();

    // these are public because I don't want the overhead of creating new functions
    static ScrollingBuffer s_UpdateFPSBuffer;
    static ScrollingBuffer s_RealUpdateFPSBuffer;
    static ScrollingBuffer s_RenderFPSBuffer;

    static ScrollingBuffer s_UpdateFTBuffer;
    static ScrollingBuffer s_RenderFTBuffer;

  private:
    static float m_UpdateTime;
    static float m_RenderTime;
};

#endif  // INCLUDE_IMGUICORE_GAMEFPSTRACKER_H_
