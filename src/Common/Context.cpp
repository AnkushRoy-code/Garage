#include "Context.h"

#include "SDL_Exception.h"

#include <SDL3/SDL_gpu.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw SDL_Exception("Couldn't initialize SDL");
    }

    mDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV
                                      | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!mDevice) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    mWindow = SDL_CreateWindow("Ankush's Garage", 640, 480,
                               SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!mWindow) { throw SDL_Exception("Unable to create SDL_Window"); }

    if (!SDL_ClaimWindowForGPUDevice(mDevice, mWindow))
    {
        throw SDL_Exception("Unable to claim window for device");
    }

    SDL_SetGPUSwapchainParameters(mDevice, mWindow,
                                  SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                  SDL_GPU_PRESENTMODE_MAILBOX);
    // ImGui Setup

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiTreeNodeFlags_DefaultOpen;

    ImGui::StyleColorsDark();

    ImGuiStyle &style        = ImGui::GetStyle();
    style.FrameRounding      = 2.0f;
    style.GrabRounding       = 2.0f;
    style.WindowBorderSize   = 0.0f;
    style.WindowTitleAlign   = ImVec2(0.5f, 0.5f);
    style.WindowRounding     = 4.0f;
    style.SeparatorTextAlign = ImVec2(0.5f, 0.5f);

    ImGui_ImplSDL3_InitForSDLGPU(mWindow);
    ImGui_ImplSDLGPU3_InitInfo init_info {};
    init_info.Device = mDevice;
    init_info.ColorTargetFormat =
        SDL_GetGPUSwapchainTextureFormat(mDevice, mWindow);
    init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);
}

Context::~Context()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();
    SDL_ReleaseWindowFromGPUDevice(mDevice, mWindow);
}
