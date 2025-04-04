#include "SimpleExample.h"
#include "Projects/Common/Common.h"
#include "Core/Console.h"
#include "Core/Context.h"
#include <iostream>

bool SimpleExample::Init()
{
    hasUI = false;
    Core::ConsoleLogBuffer::addMessage("SimpleExample Initialised\n");
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
        SDL_AcquireGPUCommandBuffer(gContext.renderData.device);
    if (!commandBufferProjects) { std::cerr << "failed to aquire GPU\n"; }

    const SDL_GPUColorTargetInfo projectTargetInfo {
        .texture     = gContext.renderData.projectTexture,
        .clear_color = SDL_FColor {0.45f, 0.55f, 0.60f, 1.00f},
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .cycle       = true,
    };

    gContext.renderData.projectPass =
        SDL_BeginGPURenderPass(commandBufferProjects, &projectTargetInfo, 1, nullptr);

    SDL_EndGPURenderPass(gContext.renderData.projectPass);
    SDL_SubmitGPUCommandBuffer(commandBufferProjects);
    return true;
}

void SimpleExample::Quit() {}
