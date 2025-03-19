#ifndef INCLUDE_TRIANGLE_TRIANGLE_H_

#define INCLUDE_TRIANGLE_TRIANGLE_H_

#include "Common/BaseProject.h"
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

    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    std::string name = "Triangle";
    [[nodiscard]] const std::string &getName() override {
        return name;
    }
};

#endif  // INCLUDE_TRIANGLE_TRIANGLE_H_
