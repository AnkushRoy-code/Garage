#include "Context.h"

#include "SDL_Exception.h"

#include <SDL3/SDL_gpu.h>
#include <print>
#include <utility>

void Context::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) { throw SDL_Exception("Couldn't initialize SDL"); }

    mDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL
                                      | SDL_GPU_SHADERFORMAT_MSL,
                                  false, nullptr);

    if (!mDevice) { throw SDL_Exception("Unable to create SDL_GPUDevice"); }

    mWindow = SDL_CreateWindow("SDL_GPU_API_TRY", 640, 480, 0);

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
    instances->push_back(
        {name, std::move(init), std::move(update), std::move(draw), std::move(quit)});
}
