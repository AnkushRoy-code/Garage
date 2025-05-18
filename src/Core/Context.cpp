#include "Core/Context.h"

#include "Core/Common/Common.h"
#include "Core/ImGuiCore/ImGuiCore.h"
#include "Core/Renderer/Renderer.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
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
    if (!SDL_SetAppMetadata("Ankush's Garage", "0.2.7", "io.github.ankushroy-code.garage"))
    {
        std::cerr << "Couldn't do stuff... 0983\n";
        return;
    }

    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Ankush Roy");
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_COPYRIGHT_STRING,
        "Placed in the public domain");  // should I stay away from stuff I don't understand?
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING,
                               "https://ankushroy-code.github.io/Garage/");
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "game");

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

    // if (!SDL_SetWindowIcon(RenderData.Window, Common::LoadImage("Logo/AnkushGarage")))
    // {
    //     std::cerr << "Unable to set window Icon!!\n";
    //     return;
    // }

    if (!SDL_ClaimWindowForGPUDevice(RenderData.Device, RenderData.Window))
    {
        std::cerr << "Unable to claim window for device\n";
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
