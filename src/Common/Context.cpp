#include "Context.h"

#include "SDL_Exception.h"

#include <SDL3/SDL_gpu.h>
#include <utility>

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw SDL_Exception("Couldn't initialize SDL"); }

    mDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!mDevice) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    mWindow = SDL_CreateWindow("Ankush's Garage", 640, 480, 0);

    if (!mWindow) { throw SDL_Exception("Unable to create SDL_Window"); }

    if (!SDL_ClaimWindowForGPUDevice(mDevice, mWindow))
    {
        throw SDL_Exception("Unable to claim window for device");
    }
}

Context::~Context()
{
    SDL_ReleaseWindowFromGPUDevice(mDevice, mWindow);
}
