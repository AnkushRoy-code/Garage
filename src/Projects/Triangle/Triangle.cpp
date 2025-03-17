#include "Projects/Triangle/Triangle.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"

#include "Core/Console.h"
#include "Core/Context.h"

SDL_GPUViewport Triangle::SmallViewport = {160, 120, 320, 240, 0.1f, 1.0f};
SDL_Rect Triangle::ScissorRect          = {320, 240, 320, 240};

SDL_GPUGraphicsPipeline *Triangle::FillPipeline {};
SDL_GPUGraphicsPipeline *Triangle::LinePipeline {};

bool Triangle::UseWireframeMode = false;
bool Triangle::UseSmallViewport = false;
bool Triangle::UseScissorRect   = false;

bool Triangle::Init()
{
    PROFILE_SCOPE_N("Triangle::Init");
    Name = "Triangle";

    SDL_GPUShader *vertexShader =
        Common::LoadShader(gContext.mDevice, "RawTriangle.vert", 0, 0, 0, 0);
    if (vertexShader == nullptr) { throw SDL_Exception("Failed to create vertex shader!"); }

    SDL_GPUShader *fragmentShader =
        Common::LoadShader(gContext.mDevice, "SolidColor.frag", 0, 0, 0, 0);
    if (fragmentShader == nullptr) { throw SDL_Exception("Failed to create fragment shader!"); }

    SDL_GPUColorTargetDescription colorTargetDesc = {};
    colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(gContext.mDevice, gContext.mWindow);

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {};
    pipelineCreateInfo.vertex_shader                         = vertexShader;
    pipelineCreateInfo.fragment_shader                       = fragmentShader;
    pipelineCreateInfo.primitive_type                        = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pipelineCreateInfo.target_info.color_target_descriptions = &colorTargetDesc;
    pipelineCreateInfo.target_info.num_color_targets         = 1;

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    FillPipeline = SDL_CreateGPUGraphicsPipeline(gContext.mDevice, &pipelineCreateInfo);
    if (FillPipeline == nullptr) { throw SDL_Exception("Failed to create fill pipeline!"); }

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
    LinePipeline = SDL_CreateGPUGraphicsPipeline(gContext.mDevice, &pipelineCreateInfo);
    if (LinePipeline == nullptr) { throw SDL_Exception("Failed to create line pipeline!"); }

    SDL_ReleaseGPUShader(gContext.mDevice, vertexShader);
    SDL_ReleaseGPUShader(gContext.mDevice, fragmentShader);

    ConsoleLogBuffer::addMessage("Triangle Initialised\n"
                                 "Press Left to toggle wireframe mode\n"
                                 "Press Down to toggle small viewport\n"
                                 "Press Right to toggle scissor rect");

    return true;
}

bool Triangle::Update()
{
    PROFILE_SCOPE_N("Triangle::Update");
    // clang-format off
    if (gContext.left) { UseWireframeMode = !UseWireframeMode; gContext.left = false; }
    if (gContext.down) { UseSmallViewport = !UseSmallViewport; gContext.down = false; }
    if (gContext.right) { UseScissorRect = !UseScissorRect; gContext.right = false; }
    // clang-format on

    return true;
}

bool Triangle::Draw()
{
    PROFILE_SCOPE_N("Triangle::Draw");
    SDL_BindGPUGraphicsPipeline(gContext.mProjectPass,
                                UseWireframeMode ? LinePipeline : FillPipeline);
    if (UseSmallViewport) { SDL_SetGPUViewport(gContext.mProjectPass, &SmallViewport); }
    if (UseScissorRect) { SDL_SetGPUScissor(gContext.mProjectPass, &ScissorRect); }
    SDL_DrawGPUPrimitives(gContext.mProjectPass, 3, 1, 0, 0);

    return true;
}

void Triangle::Quit()
{
    PROFILE_SCOPE_N("Triangle::Quit");
    SDL_ReleaseGPUGraphicsPipeline(gContext.mDevice, FillPipeline);
    SDL_ReleaseGPUGraphicsPipeline(gContext.mDevice, LinePipeline);

    UseWireframeMode = false;
    UseSmallViewport = false;
    UseScissorRect   = false;
}
