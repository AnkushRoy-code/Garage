#include "Projects/Triangle/Triangle.h"

#include "Core/Common/Common.h"
#include "Core/Console.h"
#include "Core/Context.h"
#include "Core/Event/EventHandler.h"

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

    auto &rndt = Core::Context::GetContext()->RenderData;

    auto vertexShader =
        Common::LoadShader(rndt.Device, "RawTriangle.vert", 0, 0, 0, 0)
            .or_else([](const std::string &error) -> std::expected<SDL_GPUShader *, std::string>
                     {
                         return std::expected<SDL_GPUShader *, std::string> {};  // put a default value for this type of shader
                     });

    auto fragmentShader =
        Common::LoadShader(rndt.Device, "SolidColor.frag", 0, 0, 0, 0)
            .or_else([](const std::string &error) -> std::expected<SDL_GPUShader *, std::string>
                     {
                         return std::expected<SDL_GPUShader *, std::string> {};  // put a default value for this type of shader
                     });

    assert(vertexShader);
    assert(fragmentShader);

    const SDL_GPUColorTargetDescription colorTargetDesc = {
        .format = SDL_GetGPUSwapchainTextureFormat(rndt.Device, rndt.Window),
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {
        .vertex_shader   = *vertexShader,
        .fragment_shader = *fragmentShader,
        .primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    };

    pipelineCreateInfo.target_info.color_target_descriptions = &colorTargetDesc,
    pipelineCreateInfo.target_info.num_color_targets         = 1,

    pipelineCreateInfo.multisample_state.sample_count = rndt.SampleCount;

    // the two pipelines only differ in fill_mode
    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

    FillPipeline = SDL_CreateGPUGraphicsPipeline(rndt.Device, &pipelineCreateInfo);
    assert(FillPipeline);

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;

    LinePipeline = SDL_CreateGPUGraphicsPipeline(rndt.Device, &pipelineCreateInfo);
    assert(LinePipeline);

    SDL_ReleaseGPUShader(rndt.Device, *vertexShader);
    SDL_ReleaseGPUShader(rndt.Device, *fragmentShader);

    static bool firstTime = true;
    if (firstTime)
    {
        Core::ConsoleLogBuffer::AddMessage("Triangle Initialised\n"
                                           "Press Left to toggle wireframe mode\n"
                                           "Press Down to toggle small viewport\n"
                                           "Press Right to toggle scissor rect");
        firstTime = false;
    }

    return true;
}

bool Triangle::Update()
{
    PROFILE_SCOPE_N("Triangle::Update");
    auto &event = Core::Context::GetContext()->EventHandler;
    if (event.Keys[Core::LEFT] == Core::PRESSED) { UseWireframeMode = !UseWireframeMode; }
    if (event.Keys[Core::DOWN] == Core::PRESSED) { UseSmallViewport = !UseSmallViewport; }
    if (event.Keys[Core::RIGHT] == Core::PRESSED) { UseScissorRect = !UseScissorRect; }

    return true;
}

bool Triangle::Draw()
{
    PROFILE_SCOPE_N("Triangle::Draw");
    auto &rndt = Core::Context::GetContext()->RenderData;

    SDL_GPUCommandBuffer *commandBufferProjects = SDL_AcquireGPUCommandBuffer(rndt.Device);
    if (!commandBufferProjects) { std::cerr << "failed to aquire GPU\n"; }

    SDL_GPUColorTargetInfo projectTargetInfo {
        .texture     = rndt.ProjectTexture,
        .clear_color = {0.45f, 0.55f, 0.60f, 1.00f},
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .cycle       = true,
    };

    if (rndt.SampleCount == SDL_GPU_SAMPLECOUNT_1)
    {
        projectTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    }
    else
    {
        projectTargetInfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
        projectTargetInfo.resolve_texture = rndt.ResolveTexture;
    }

    rndt.ProjectPass =
        SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(rndt.ProjectPass, UseWireframeMode ? LinePipeline : FillPipeline);
    if (UseSmallViewport) { SDL_SetGPUViewport(rndt.ProjectPass, &SmallViewport); }
    if (UseScissorRect) { SDL_SetGPUScissor(rndt.ProjectPass, &ScissorRect); }
    SDL_DrawGPUPrimitives(rndt.ProjectPass, 3, 1, 0, 0);

    SDL_EndGPURenderPass(rndt.ProjectPass);

    SDL_GPUTexture *blitSourceTexture = (projectTargetInfo.resolve_texture != nullptr)
                                            ? projectTargetInfo.resolve_texture
                                            : projectTargetInfo.texture;

    const SDL_GPUBlitRegion blitSrc = {
        .texture = blitSourceTexture,
        .w       = (Uint32)Core::Context::GetContext()->AppState.ProjectWindowSize.x,
        .h       = (Uint32)Core::Context::GetContext()->AppState.ProjectWindowSize.y,
    };

    const SDL_GPUBlitRegion blitDst = {
        .texture = rndt.ProjectTexture,
        .w       = (Uint32)Core::Context::GetContext()->AppState.ProjectWindowSize.x,
        .h       = (Uint32)Core::Context::GetContext()->AppState.ProjectWindowSize.y,
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

    auto &rndt = Core::Context::GetContext()->RenderData;

    SDL_ReleaseGPUGraphicsPipeline(rndt.Device, FillPipeline);
    SDL_ReleaseGPUGraphicsPipeline(rndt.Device, LinePipeline);

    UseWireframeMode = false;
    UseSmallViewport = false;
    UseScissorRect   = false;
}
