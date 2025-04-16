#include "Core/ImGuiCore/FPSTracker.h"

#include <SDL3/SDL_timer.h>

ScrollingBuffer Tracker::s_UpdateFPSBuffer {};
ScrollingBuffer Tracker::s_RealUpdateFPSBuffer {};
ScrollingBuffer Tracker::s_RenderFPSBuffer {};

ScrollingBuffer Tracker::s_UpdateFTBuffer {};
ScrollingBuffer Tracker::s_RenderFTBuffer {};

float Tracker::m_UpdateTime {};
float Tracker::m_RenderTime {};

void Tracker::AddUpdateFPSPointQueue(float UpdateTime)
{
    m_UpdateTime = UpdateTime;
}

void Tracker::AddRenderFPSPointQueue(float RenderTime)
{
    m_RenderTime = RenderTime;
}

void Tracker::RegisterPoints()
{
    const auto t = SDL_GetTicks() / 1000.0f;

    s_UpdateFTBuffer.AddPoint(t, m_UpdateTime);
    s_RenderFTBuffer.AddPoint(t, m_RenderTime);

    m_RenderTime = 1000.0f / m_RenderTime;
    m_UpdateTime = 1000.0f / m_UpdateTime;

    s_UpdateFPSBuffer.AddPoint(t, m_UpdateTime);
    s_RealUpdateFPSBuffer.AddPoint(t, (m_UpdateTime < 60.0f ? m_UpdateTime : 60));
    s_RenderFPSBuffer.AddPoint(t, m_RenderTime);

    m_UpdateTime = 0;
    m_RenderTime = 0;
}
