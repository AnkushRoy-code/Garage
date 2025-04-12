#include "Core/ImGuiCore/ScrollingBuffer.h"

ScrollingBuffer::ScrollingBuffer(int p_MaxSize)
{
    MaxSize = p_MaxSize;
    Offset  = 0;
    Data.reserve(MaxSize);
}

int ScrollingBuffer::GetOffset() const
{
    return (Offset + 1) % MaxSize;
}

void ScrollingBuffer::AddPoint(float x, float y)
{
    if (Data.size() < MaxSize)
        Data.push_back(ImVec2(x, y));
    else
    {
        Data[Offset] = ImVec2(x, y);
        Offset       = (Offset + 1) % MaxSize;
    }
}

void ScrollingBuffer::Erase()
{
    if (Data.size() > 0)
    {
        Data.shrink(0);
        Offset = 0;
    }
}
