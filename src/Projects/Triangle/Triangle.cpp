#include "Projects/Triangle/Triangle.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"

#include "Core/Console.h"
#include "Core/Context.h"
#include "Core/EventHandler.h"
#include "SDL3/SDL_gpu.h"
#include <iostream>

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
    hasUI = false;

    SDL_GPUShader *vertexShader =
        Common::LoadShader(gContext.renderData.device, "RawTriangle.vert", 0, 0, 0, 0);
    if (vertexShader == nullptr) { throw SDL_Exception("Failed to create vertex shader!"); }

    SDL_GPUShader *fragmentShader =
        Common::LoadShader(gContext.renderData.device, "SolidColor.frag", 0, 0, 0, 0);
    if (fragmentShader == nullptr) { throw SDL_Exception("Failed to create fragment shader!"); }

    const SDL_GPUColorTargetDescription colorTargetDesc = {
        .format = SDL_GetGPUSwapchainTextureFormat(gContext.renderData.device,
                                                   gContext.renderData.window),
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {
        .vertex_shader   = vertexShader,
        .fragment_shader = fragmentShader,
        .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    };

    pipelineCreateInfo.target_info.color_target_descriptions = &colorTargetDesc,
    pipelineCreateInfo.target_info.num_color_targets         = 1,

    pipelineCreateInfo.multisample_state.sample_count = gContext.renderData.sampleCount;

    // the two pipelines only differ in fill_mode
    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

    FillPipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &pipelineCreateInfo);
    if (FillPipeline == nullptr) { throw SDL_Exception("Failed to create fill pipeline!"); }

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;

    LinePipeline = SDL_CreateGPUGraphicsPipeline(gContext.renderData.device, &pipelineCreateInfo);
    if (LinePipeline == nullptr) { throw SDL_Exception("Failed to create line pipeline!"); }

    SDL_ReleaseGPUShader(gContext.renderData.device, vertexShader);
    SDL_ReleaseGPUShader(gContext.renderData.device, fragmentShader);

    Core::ConsoleLogBuffer::addMessage("Triangle Initialised\n"
                                       "Press Left to toggle wireframe mode\n"
                                       "Press Down to toggle small viewport\n"
                                       "Press Right to toggle scissor rect");

    return true;
}

bool Triangle::Update()
{
    PROFILE_SCOPE_N("Triangle::Update");
    // clang-format off
    if (gContext.inputHandler.Keys[Core::LEFT] == Core::PRESSED) { UseWireframeMode = !UseWireframeMode; }
    if (gContext.inputHandler.Keys[Core::DOWN] == Core::PRESSED) { UseSmallViewport = !UseSmallViewport; }
    if (gContext.inputHandler.Keys[Core::RIGHT] == Core::PRESSED) { UseScissorRect = !UseScissorRect; }
    // clang-format on

    return true;
}

bool Triangle::Draw()
{
    PROFILE_SCOPE_N("Triangle::Draw");

    SDL_GPUCommandBuffer *commandBufferProjects =
        SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (!commandBufferProjects) { std::cerr << "failed to aquire GPU\n"; }

    SDL_GPUColorTargetInfo projectTargetInfo {
        .texture     = gContext.renderData.projectTexture,
        .clear_color = {0.45f, 0.55f, 0.60f, 1.00f},
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .cycle       = true,
    };

    if (gContext.renderData.sampleCount == SDL_GPU_SAMPLECOUNT_1)
    {
        projectTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    }
    else
    {
        projectTargetInfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
        projectTargetInfo.resolve_texture = gContext.renderData.resolveTexture;
    }

    gContext.renderData.projectPass =
        SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(gContext.renderData.projectPass,
                                UseWireframeMode ? LinePipeline : FillPipeline);
    if (UseSmallViewport) { SDL_SetGPUViewport(gContext.renderData.projectPass, &SmallViewport); }
    if (UseScissorRect) { SDL_SetGPUScissor(gContext.renderData.projectPass, &ScissorRect); }
    SDL_DrawGPUPrimitives(gContext.renderData.projectPass, 3, 1, 0, 0);

    SDL_EndGPURenderPass(gContext.renderData.projectPass);

    SDL_GPUTexture *blitSourceTexture = (projectTargetInfo.resolve_texture != nullptr)
                                            ? projectTargetInfo.resolve_texture
                                            : projectTargetInfo.texture;

    const SDL_GPUBlitRegion blitSrc = {
        .texture = blitSourceTexture,
        .w       = (Uint32)gContext.appState.ProjectWindowX,
        .h       = (Uint32)gContext.appState.ProjectWindowY,
    };

    const SDL_GPUBlitRegion blitDst = {
        .texture = gContext.renderData.projectTexture,
        .w       = (Uint32)gContext.appState.ProjectWindowX,
        .h       = (Uint32)gContext.appState.ProjectWindowY,
    };

    const SDL_GPUBlitInfo blitInfo = {.source      = blitSrc,
                                      .destination = blitDst,
                                      .load_op     = SDL_GPU_LOADOP_DONT_CARE,
                                      .filter      = SDL_GPU_FILTER_LINEAR};

    SDL_BlitGPUTexture(commandBufferProjects, &blitInfo);

    SDL_SubmitGPUCommandBuffer(commandBufferProjects);

    return true;
}

void Triangle::Quit()
{
    PROFILE_SCOPE_N("Triangle::Quit");
    SDL_ReleaseGPUGraphicsPipeline(gContext.renderData.device, FillPipeline);
    SDL_ReleaseGPUGraphicsPipeline(gContext.renderData.device, LinePipeline);

    UseWireframeMode = false;
    UseSmallViewport = false;
    UseScissorRect   = false;
}
