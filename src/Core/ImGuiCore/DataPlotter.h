#ifndef INCLUDE_IMGUICORE_DATAPLOTTER_H_
#define INCLUDE_IMGUICORE_DATAPLOTTER_H_

#include "Core/Common/pch.h"

class Plotter
{
  public:
    static void PlotAllData();

  private:
    static void PlotFrameTimeData();
    static void PlotFPSData();

  private:
    static float m_History;
    static float m_Ticks;
    static constexpr ImVec4 m_UpdateCol = {237.0f / 255.0f, 135.0f / 255.0f, 150.0f / 255.0f, 1.0f};
    static constexpr ImVec4 m_RenderCol = {145.0f / 255.0f, 215.0f / 255.0f, 227.0f / 255.0f, 1.0f};
};

#endif  // INCLUDE_IMGUICORE_DATAPLOTTER_H_
