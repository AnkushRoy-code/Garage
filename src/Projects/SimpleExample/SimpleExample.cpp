#include "Projects/SimpleExample/SimpleExample.h"

#include "Core/Common/Common.h"
#include "Core/Console.h"
#include "Core/Context.h"

#include <iostream>

bool SimpleExample::Init()
{
    hasUI = false;
    Core::ConsoleLogBuffer::AddMessage("SimpleExample Initialised\n");
    return true;
}
bool SimpleExample::Update()
{
    return true;
}

bool SimpleExample::Draw()
{
    PROFILE_SCOPE_N("SimpleExample::Draw");

    SDL_GPUCommandBuffer *commandBufferProjects =
        SDL_AcquireGPUCommandBuffer(Core::Context::GetContext()->RenderData.Device);
    assert(commandBufferProjects);

    const SDL_GPUColorTargetInfo projectTargetInfo {
        .texture     = Core::Context::GetContext()->RenderData.ProjectTexture,
        .clear_color = SDL_FColor {0.45f, 0.55f, 0.60f, 1.00f},
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .cycle       = true,
    };

    Core::Context::GetContext()->RenderData.ProjectPass =
        SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

    SDL_EndGPURenderPass(Core::Context::GetContext()->RenderData.ProjectPass);
    SDL_SubmitGPUCommandBuffer(commandBufferProjects);
    return true;
}

void SimpleExample::Quit() {}
