#include "Context.h"
#include <SDL3/SDL_gpu.h>
#include <print>

bool Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::println("Couldn't initialize SDL: {}", SDL_GetError());
        return false;
    }

    mDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, NULL);

    if (!mDevice)
    {
        std::println("Unable to create SDL_GPUDevice. SDL_Error: {}", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("SDL_GPU_API_TRY", 640, 480, 0);

    if (!mWindow)
    {
        std::println("Unable to create SDL_Window. SDL_Error: {}", SDL_GetError());
        return false;
    }

    if (!SDL_ClaimWindowForGPUDevice(mDevice, mWindow))
    {
        std::println("Unable to claim window for device. SDL_Error: {}", SDL_GetError());
        return false;
    }

    return true;
}

Context::~Context()
{
    SDL_ReleaseWindowFromGPUDevice(mDevice, mWindow);
}

std::unique_ptr<std::vector<Example>> ExampleRegistry::instances = nullptr;

const std::vector<Example> &ExampleRegistry::getAllExamples()
{
    if (!instances) { instances = std::make_unique<std::vector<Example>>(); }
    return *instances;
}

void ExampleRegistry::registerExample(const std::string &name,
                                      std::function<bool(Context &)> init,
                                      std::function<bool(Context &)> update,
                                      std::function<bool(Context &)> draw,
                                      std::function<void(Context &)> quit)
{
    if (!instances) instances = std::make_unique<std::vector<Example>>();
    instances->push_back({name, init, update, draw, quit});
}
