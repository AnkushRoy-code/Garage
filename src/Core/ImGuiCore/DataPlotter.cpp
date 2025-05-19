#include "Core/ImGuiCore/DataPlotter.h"
#include "Core/ImGuiCore/FPSTracker.h"

#include <implot.h>

float Plotter::m_History = 10.0f;
float Plotter::m_Ticks   = 0.0f;

void Plotter::PlotAllData()
{
    ImGui::SliderFloat("History", &m_History, 1, 30, "%.1f s");
    m_Ticks = SDL_GetTicks() / 1000.0f;

    PlotFrameTimeData();
    PlotFPSData();
}

void Plotter::PlotFrameTimeData()
{
    constexpr ImPlotAxisFlags flags =
        ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;

    if (ImPlot::BeginPlot("Frame Time Plot", ImVec2(-1, 250),
                          ImPlotFlags_NoBoxSelect | ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupLegend(ImPlotLocation_SouthEast);
        ImPlot::SetupMouseText(ImPlotLocation_SouthWest);
        ImPlot::SetupAxes("Time (s)", "Frame Time (ms)", flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, m_Ticks - m_History, m_Ticks, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 65);
        ImPlot::SetupFinish();

        ImPlot::PushStyleColor(ImPlotCol_Line, m_UpdateCol);
        ImPlot::PlotLine("Update", &Tracker::s_UpdateFTBuffer.Data[0].x,
                         &Tracker::s_UpdateFTBuffer.Data[0].y,
                         Tracker::s_UpdateFTBuffer.Data.size(), 0, Tracker::s_UpdateFTBuffer.Offset,
                         2 * sizeof(float));
        ImPlot::PopStyleColor();

        ImPlot::PushStyleColor(ImPlotCol_Line, m_RenderCol);
        ImPlot::PlotLine("Render", &Tracker::s_RenderFTBuffer.Data[0].x,
                         &Tracker::s_RenderFTBuffer.Data[0].y,
                         Tracker::s_RenderFTBuffer.Data.size(), 0, Tracker::s_RenderFTBuffer.Offset,
                         2 * sizeof(float));

        ImPlot::EndPlot();
    }
}

void Plotter::PlotFPSData()
{
    constexpr ImPlotAxisFlags flags =
        ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;

    if (ImPlot::BeginPlot("FPS Plot", ImVec2(-1, 250),
                          ImPlotFlags_NoBoxSelect | ImPlotFlags_Crosshairs))
    {
        ImPlot::SetupLegend(ImPlotLocation_SouthEast);
        ImPlot::SetupMouseText(ImPlotLocation_SouthWest);
        ImPlot::SetupAxes("Time (s)", "FPS", flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, m_Ticks - m_History, m_Ticks, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 65);
        ImPlot::SetupFinish();

        ImPlot::PushStyleColor(ImPlotCol_Line, m_UpdateCol);
        ImPlot::PlotLine("Update", &Tracker::s_UpdateFPSBuffer.Data[0].x,
                         &Tracker::s_UpdateFPSBuffer.Data[0].y,
                         Tracker::s_UpdateFPSBuffer.Data.size(), 0,
                         Tracker::s_UpdateFPSBuffer.Offset, 2 * sizeof(float));
        ImPlot::PopStyleColor();

        ImPlot::PushStyleColor(ImPlotCol_Line,
                               {166.0f / 255.0f, 218.0f / 255.0f, 149.0f / 255.0f, 1.0f});
        ImPlot::PlotLine("RealUpdate", &Tracker::s_RealUpdateFPSBuffer.Data[0].x,
                         &Tracker::s_RealUpdateFPSBuffer.Data[0].y,
                         Tracker::s_RealUpdateFPSBuffer.Data.size(), 0,
                         Tracker::s_RealUpdateFPSBuffer.Offset, 2 * sizeof(float));
        ImPlot::PopStyleColor();

        ImPlot::PushStyleColor(ImPlotCol_Line, m_RenderCol);
        ImPlot::PlotLine("Render", &Tracker::s_RenderFPSBuffer.Data[0].x,
                         &Tracker::s_RenderFPSBuffer.Data[0].y,
                         Tracker::s_RenderFPSBuffer.Data.size(), 0,
                         Tracker::s_RenderFPSBuffer.Offset, 2 * sizeof(float));
        ImPlot::PopStyleColor();

        ImPlot::EndPlot();
    }
}
