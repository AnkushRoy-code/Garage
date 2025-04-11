#ifndef INCLUDE_IMGUICORE_SCROLLINGBUFFER_H_
#define INCLUDE_IMGUICORE_SCROLLINGBUFFER_H_

#include <imgui.h>

struct ScrollingBuffer
{
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    explicit ScrollingBuffer(int p_MaxSize = 2000);
    [[nodiscard]] int GetOffset() const;
    void AddPoint(float x, float y);
    void Erase();
};

#endif  // INCLUDE_IMGUICORE_SCROLLINGBUFFER_H_
