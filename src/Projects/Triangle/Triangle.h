#ifndef INCLUDE_TRIANGLE_TRIANGLE_H_

#define INCLUDE_TRIANGLE_TRIANGLE_H_

#include "Common/BaseProject.h"
#include "Common/Context.h"
#include <SDL3/SDL_gpu.h>

class Triangle : public BaseProject
{
    static SDL_GPUViewport SmallViewport;
    static SDL_Rect ScissorRect;

    static SDL_GPUGraphicsPipeline *FillPipeline;
    static SDL_GPUGraphicsPipeline *LinePipeline;

    static bool UseWireframeMode;
    static bool UseSmallViewport;
    static bool UseScissorRect;

    bool Init(Context &context) override;
    bool Update(Context &context) override;
    bool Draw(Context &context) override;
    void Quit(Context &context) override;
};

#endif  // INCLUDE_TRIANGLE_TRIANGLE_H_
