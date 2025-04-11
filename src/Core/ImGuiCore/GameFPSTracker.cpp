#include "GameFPSTracker.h"
#include "SDL3/SDL_timer.h"

ScrollingBuffer Tracker::UpdateFPSBuffer {};
ScrollingBuffer Tracker::RealUpdateFPSBuffer {};
ScrollingBuffer Tracker::RenderFPSBuffer {};

float Tracker::updateTime {};
float Tracker::renderTime {};

void Tracker::AddUpdateFPSPointQueue(float UpdateTime)
{
    updateTime = 1000.0f / UpdateTime;
}

void Tracker::AddRenderFPSPointQueue(float RenderTime)
{
    renderTime = 1000.0f / RenderTime;
}

void Tracker::registerPoints()
{
    const auto t = SDL_GetTicks() / 1000.0f;

    UpdateFPSBuffer.AddPoint(t, updateTime);
    RealUpdateFPSBuffer.AddPoint(t, (updateTime < 60.0f ? updateTime : 60));
    RenderFPSBuffer.AddPoint(t, renderTime);

    // updateTime = 0;
    // renderTime = 0;
}
