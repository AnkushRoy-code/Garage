#include "Common/Common.h"
#include "Common/SDL_Exception.h"

#include <SDL3/SDL_gpu.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <print>

namespace Common
{
SDL_GPUShader *LoadShader(SDL_GPUDevice *device,
                          const std::string &shaderFilename,
                          const Uint32 samplerCount,
                          const Uint32 uniformBufferCount,
                          const Uint32 storageBufferCount,
                          const Uint32 storageTextureCount)
{
    SDL_GPUShaderStage stage;
    if (shaderFilename.contains(".vert"))
        stage = SDL_GPU_SHADERSTAGE_VERTEX;
    else if (shaderFilename.contains(".frag"))
        stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    else { throw SDL_Exception("Invalid shader stage, what are you trying to do?"); }

    std::filesystem::path fullPath;
    const SDL_GPUShaderFormat backendFormats {SDL_GetGPUShaderFormats(device)};
    SDL_GPUShaderFormat format {};
    std::string entrypoint {"main"};  // Because of think different.

    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        fullPath = "res/Shaders/Compiled/SPIRV/" + (shaderFilename + ".spv");
        format   = SDL_GPU_SHADERFORMAT_SPIRV;
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        fullPath   = "res/Shaders/Compiled/MSL/" + (shaderFilename + ".msl");
        format     = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        fullPath = "res/Shaders/Compiled/DXIL/" + (shaderFilename + ".dxil");
        format   = SDL_GPU_SHADERFORMAT_DXIL;
    }
    else { throw SDL_Exception("Unrecognised backend shader format"); }

    std::ifstream file {fullPath, std::ios::binary};

    if (!file) throw SDL_Exception("Couldn't open shader file");

    std::vector<Uint8> code {std::istreambuf_iterator(file), {}};

    const SDL_GPUShaderCreateInfo shaderInfo {.code_size            = code.size(),
                                              .code                 = code.data(),
                                              .entrypoint           = entrypoint.c_str(),
                                              .format               = format,
                                              .stage                = stage,
                                              .num_samplers         = samplerCount,
                                              .num_storage_textures = storageTextureCount,
                                              .num_storage_buffers  = storageBufferCount,
                                              .num_uniform_buffers  = uniformBufferCount};

    SDL_GPUShader *shader = SDL_CreateGPUShader(device, &shaderInfo);

    if (shader == nullptr) { throw SDL_Exception("Failed to create Shader"); }

    return shader;
}

}  // namespace Common
