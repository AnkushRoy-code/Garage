#include "Projects/Triangle/Triangle.h"
#include "Core/Common/Common.h"
#include "Core/Common/SDL_Exception.h"

#include "Core/Console.h"
#include "Core/Context.h"
#include "Core/EventHandler.h"

#include "Core/Common/pch.h"

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
        Common::LoadShader(g_Context.RenderData.Device, "RawTriangle.vert", 0, 0, 0, 0);
    if (vertexShader == nullptr) { throw SDL_Exception("Failed to create vertex shader!"); }

    SDL_GPUShader *fragmentShader =
        Common::LoadShader(g_Context.RenderData.Device, "SolidColor.frag", 0, 0, 0, 0);
    if (fragmentShader == nullptr) { throw SDL_Exception("Failed to create fragment shader!"); }

    const SDL_GPUColorTargetDescription colorTargetDesc = {
        .format = SDL_GetGPUSwapchainTextureFormat(g_Context.RenderData.Device,
                                                   g_Context.RenderData.Window),
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {
        .vertex_shader   = vertexShader,
        .fragment_shader = fragmentShader,
        .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    };

    pipelineCreateInfo.target_info.color_target_descriptions = &colorTargetDesc,
    pipelineCreateInfo.target_info.num_color_targets         = 1,

    pipelineCreateInfo.multisample_state.sample_count = g_Context.RenderData.SampleCount;

    // the two pipelines only differ in fill_mode
    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

    FillPipeline = SDL_CreateGPUGraphicsPipeline(g_Context.RenderData.Device, &pipelineCreateInfo);
    if (FillPipeline == nullptr) { throw SDL_Exception("Failed to create fill pipeline!"); }

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;

    LinePipeline = SDL_CreateGPUGraphicsPipeline(g_Context.RenderData.Device, &pipelineCreateInfo);
    if (LinePipeline == nullptr) { throw SDL_Exception("Failed to create line pipeline!"); }

    SDL_ReleaseGPUShader(g_Context.RenderData.Device, vertexShader);
    SDL_ReleaseGPUShader(g_Context.RenderData.Device, fragmentShader);

    Core::ConsoleLogBuffer::AddMessage("Triangle Initialised\n"
                                       "Press Left to toggle wireframe mode\n"
                                       "Press Down to toggle small viewport\n"
                                       "Press Right to toggle scissor rect");

    return true;
}

bool Triangle::Update()
{
    PROFILE_SCOPE_N("Triangle::Update");
    // clang-format off
    if (g_Context.EventHandler.Keys[Core::LEFT] == Core::PRESSED) { UseWireframeMode = !UseWireframeMode; }
    if (g_Context.EventHandler.Keys[Core::DOWN] == Core::PRESSED) { UseSmallViewport = !UseSmallViewport; }
    if (g_Context.EventHandler.Keys[Core::RIGHT] == Core::PRESSED) { UseScissorRect = !UseScissorRect; }
    // clang-format on

    return true;
}

bool Triangle::Draw()
{
    PROFILE_SCOPE_N("Triangle::Draw");

    SDL_GPUCommandBuffer *commandBufferProjects =
        SDL_AcquireGPUCommandBuffer(g_Context.RenderData.Device);
    if (!commandBufferProjects) { std::cerr << "failed to aquire GPU\n"; }

    SDL_GPUColorTargetInfo projectTargetInfo {
        .texture     = g_Context.RenderData.ProjectTexture,
        .clear_color = {0.45f, 0.55f, 0.60f, 1.00f},
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .cycle       = true,
    };

    if (g_Context.RenderData.SampleCount == SDL_GPU_SAMPLECOUNT_1)
    {
        projectTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    }
    else
    {
        projectTargetInfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
        projectTargetInfo.resolve_texture = g_Context.RenderData.ResolveTexture;
    }

    g_Context.RenderData.ProjectPass =
        SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(g_Context.RenderData.ProjectPass,
                                UseWireframeMode ? LinePipeline : FillPipeline);
    if (UseSmallViewport) { SDL_SetGPUViewport(g_Context.RenderData.ProjectPass, &SmallViewport); }
    if (UseScissorRect) { SDL_SetGPUScissor(g_Context.RenderData.ProjectPass, &ScissorRect); }
    SDL_DrawGPUPrimitives(g_Context.RenderData.ProjectPass, 3, 1, 0, 0);

    SDL_EndGPURenderPass(g_Context.RenderData.ProjectPass);

    SDL_GPUTexture *blitSourceTexture = (projectTargetInfo.resolve_texture != nullptr)
                                            ? projectTargetInfo.resolve_texture
                                            : projectTargetInfo.texture;

    const SDL_GPUBlitRegion blitSrc = {
        .texture = blitSourceTexture,
        .w       = (Uint32)g_Context.AppState.ProjectWindowX,
        .h       = (Uint32)g_Context.AppState.ProjectWindowY,
    };

    const SDL_GPUBlitRegion blitDst = {
        .texture = g_Context.RenderData.ProjectTexture,
        .w       = (Uint32)g_Context.AppState.ProjectWindowX,
        .h       = (Uint32)g_Context.AppState.ProjectWindowY,
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
    SDL_ReleaseGPUGraphicsPipeline(g_Context.RenderData.Device, FillPipeline);
    SDL_ReleaseGPUGraphicsPipeline(g_Context.RenderData.Device, LinePipeline);

    UseWireframeMode = false;
    UseSmallViewport = false;
    UseScissorRect   = false;
}
