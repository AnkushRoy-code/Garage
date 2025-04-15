#include "Core/Context.h"

#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/Renderer/Renderer.h"
#include "Utils/Time.h"

namespace Core
{

std::shared_ptr<Context> Context::instance = std::make_shared<Context>();

std::shared_ptr<Context> Context::GetContext()
{
    return instance;
}

Context::Context() = default;

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Couldn't initialize SDL!!\n";
        return;
    }

    RenderData.Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                                | SDL_GPU_SHADERFORMAT_MSL,
                                            false, nullptr);

    if (!RenderData.Device)
    {
        std::cerr << "Unable to create SDL_GPUDevice\n";
        return;
    }

    RenderData.Window = SDL_CreateWindow("Ankush's Garage", RenderData.Width, RenderData.Height,
                                         SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);

    if (!RenderData.Window)
    {
        std::cerr << "Unable to create SDL_Window!\n";
        return;
    }

    if (!SDL_ClaimWindowForGPUDevice(RenderData.Device, RenderData.Window))
    {
        std::cerr << "Unable to claim window for device!\n";
        return;
    }

    ImGuiCore::Init();
    Renderer::Init();
    Utils::Time::Init();
}

Context::~Context()
{
    ImGuiCore::Quit();
    SDL_ReleaseWindowFromGPUDevice(RenderData.Device, RenderData.Window);
}

}  // namespace Core
