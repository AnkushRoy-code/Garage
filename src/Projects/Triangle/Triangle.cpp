#include "Projects/Triangle/Triangle.h"
#include "Common/Common.h"
#include "Common/SDL_Exception.h"
#include <iostream>

SDL_GPUViewport Triangle::SmallViewport = {160, 120, 320, 240, 0.1f, 1.0f};
SDL_Rect Triangle::ScissorRect          = {320, 240, 320, 240};

SDL_GPUGraphicsPipeline *Triangle::FillPipeline {};
SDL_GPUGraphicsPipeline *Triangle::LinePipeline {};

bool Triangle::UseWireframeMode = false;
bool Triangle::UseSmallViewport = false;
bool Triangle::UseScissorRect   = false;

bool Triangle::Init(Context &context)
{
    SDL_GPUShader *vertexShader =
        Common::LoadShader(context.mDevice, "RawTriangle.vert", 0, 0, 0, 0);
    if (vertexShader == nullptr) { throw SDL_Exception("Failed to create vertex shader!"); }

    SDL_GPUShader *fragmentShader =
        Common::LoadShader(context.mDevice, "SolidColor.frag", 0, 0, 0, 0);
    if (fragmentShader == nullptr) { throw SDL_Exception("Failed to create fragment shader!"); }

    SDL_GPUColorTargetDescription colorTargetDesc = {};
    colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(context.mDevice, context.mWindow);

    SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo {};
    pipelineCreateInfo.vertex_shader                         = vertexShader;
    pipelineCreateInfo.fragment_shader                       = fragmentShader;
    pipelineCreateInfo.primitive_type                        = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
    pipelineCreateInfo.target_info.color_target_descriptions = &colorTargetDesc;
    pipelineCreateInfo.target_info.num_color_targets         = 1;

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
    FillPipeline = SDL_CreateGPUGraphicsPipeline(context.mDevice, &pipelineCreateInfo);
    if (FillPipeline == nullptr) { throw SDL_Exception("Failed to create fill pipeline!"); }

    pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_LINE;
    LinePipeline = SDL_CreateGPUGraphicsPipeline(context.mDevice, &pipelineCreateInfo);
    if (LinePipeline == nullptr) { throw SDL_Exception("Failed to create line pipeline!"); }

    SDL_ReleaseGPUShader(context.mDevice, vertexShader);
    SDL_ReleaseGPUShader(context.mDevice, fragmentShader);

    std::cout << "Triangle Initialised\n";
    std::cout << "Press Left to toggle wireframe mode\n";
    std::cout << "Press Down to toggle small viewport\n";
    std::cout << "Press Right to toggle scissor rect\n";
    return true;
}

bool Triangle::Update(Context &context)
{
    // clang-format off
    if (context.left) { UseWireframeMode = !UseWireframeMode; context.left = false; }
    if (context.down) { UseSmallViewport = !UseSmallViewport; context.down = false; }
    if (context.right) { UseScissorRect = !UseScissorRect; context.right = false; }
    // clang-format on

    return true;
}

bool Triangle::Draw(Context &context)
{
    SDL_GPUCommandBuffer *cmdbuf = SDL_AcquireGPUCommandBuffer(context.mDevice);
    if (cmdbuf == nullptr) { throw SDL_Exception("AcquireGPUCommandBuffer failed!"); }

    SDL_GPUTexture *swapchainTexture;
    if (!SDL_AcquireGPUSwapchainTexture(cmdbuf, context.mWindow, &swapchainTexture, nullptr,
                                        nullptr))
    {
        throw SDL_Exception("WaitAndAcquireGPUSwapchainTexture failed!");
    }

    if (swapchainTexture != nullptr)
    {
        SDL_GPUColorTargetInfo colorTargetInfo = {nullptr};
        colorTargetInfo.texture                = swapchainTexture;
        colorTargetInfo.clear_color            = SDL_FColor {0.0f, 0.0f, 0.0f, 1.0f};
        colorTargetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op               = SDL_GPU_STOREOP_STORE;

        SDL_GPURenderPass *renderPass =
            SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, nullptr);
        SDL_BindGPUGraphicsPipeline(renderPass, UseWireframeMode ? LinePipeline : FillPipeline);
        if (UseSmallViewport) { SDL_SetGPUViewport(renderPass, &SmallViewport); }
        if (UseScissorRect) { SDL_SetGPUScissor(renderPass, &ScissorRect); }
        SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);
    return true;
}

void Triangle::Quit(Context &context)
{
    SDL_ReleaseGPUGraphicsPipeline(context.mDevice, FillPipeline);
    SDL_ReleaseGPUGraphicsPipeline(context.mDevice, LinePipeline);

    UseWireframeMode = false;
    UseSmallViewport = false;
    UseScissorRect   = false;
}
