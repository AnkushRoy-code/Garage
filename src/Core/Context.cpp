#include "Context.h"

#include "SDL3/SDL_video.h"
#include "Common/SDL_Exception.h"

#include <SDL3/SDL_gpu.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

Context gContext;

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw SDL_Exception("Couldn't initialize SDL"); }

    mDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!mDevice) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    // window Ratio:- 4:3 -> (4 + 1):3 taking into account the toolbox
    mWindow = SDL_CreateWindow("Ankush's Garage", 800, 480,
                               SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE);

    if (!mWindow) { throw SDL_Exception("Unable to create SDL_Window"); }

    if (!SDL_ClaimWindowForGPUDevice(mDevice, mWindow))
    {
        throw SDL_Exception("Unable to claim window for device");
    }

    SDL_SetGPUSwapchainParameters(mDevice, mWindow, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                  SDL_GPU_PRESENTMODE_MAILBOX);

    SDL_GPUTextureCreateInfo gpuTextureCreateInfo = {};
    gpuTextureCreateInfo.type                     = SDL_GPU_TEXTURETYPE_2D;
    gpuTextureCreateInfo.format                   = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    gpuTextureCreateInfo.usage  = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
    gpuTextureCreateInfo.width  = 640;
    gpuTextureCreateInfo.height = 480;
    gpuTextureCreateInfo.layer_count_or_depth = 1;
    gpuTextureCreateInfo.num_levels           = 1;
    gpuTextureCreateInfo.sample_count         = SDL_GPU_SAMPLECOUNT_1;

    mProjectTexture = SDL_CreateGPUTexture(mDevice, &gpuTextureCreateInfo);

    if (!mProjectTexture) { throw SDL_Exception("Unable to create GPU Texture"); }

    SDL_GPUSamplerCreateInfo samplerInfo {};
    samplerInfo.min_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mag_filter     = SDL_GPU_FILTER_NEAREST,
    samplerInfo.mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,

    mProjectSampler            = SDL_CreateGPUSampler(mDevice, &samplerInfo);

    // ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);
    io.DisplaySize = ImVec2((float)w, (float)h);

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
    init_info.Device            = mDevice;
    init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(mDevice, mWindow);
    init_info.MSAASamples       = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&init_info);
}

Context::~Context()
{
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();
    SDL_ReleaseWindowFromGPUDevice(mDevice, mWindow);
}
