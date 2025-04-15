#include "Core/Common/Common.h"

namespace Common
{
SDL_GPUShader *LoadShader(SDL_GPUDevice *device,
                          const std::string &shaderFilename,
                          const Uint32 samplerCount,
                          const Uint32 uniformBufferCount,
                          const Uint32 storageBufferCount,
                          const Uint32 storageTextureCount)
{
    static bool x_BasePathFound = false;
    static std::string x_BasePath {};

    if (!x_BasePathFound)
    {
        x_BasePath      = SDL_GetBasePath();
        x_BasePathFound = true;
    }

    SDL_GPUShaderStage stage;
    if (shaderFilename.contains(".vert")) { stage = SDL_GPU_SHADERSTAGE_VERTEX; }
    else if (shaderFilename.contains(".frag")) { stage = SDL_GPU_SHADERSTAGE_FRAGMENT; }

    // Only accept vertex or fragment shader
    assert(shaderFilename.contains(".vert") || shaderFilename.contains(".frag"));

    std::filesystem::path fullPath;
    const SDL_GPUShaderFormat backendFormats {SDL_GetGPUShaderFormats(device)};
    SDL_GPUShaderFormat format {};
    std::string entrypoint {"main"};  // Because of think different.

    bool formatFound = false;
    if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        fullPath    = x_BasePath + "res/Shaders/Compiled/SPIRV/" + (shaderFilename + ".spv");
        format      = SDL_GPU_SHADERFORMAT_SPIRV;
        formatFound = true;
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
    {
        fullPath    = x_BasePath + "res/Shaders/Compiled/MSL/" + (shaderFilename + ".msl");
        format      = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint  = "main0";
        formatFound = true;
    }
    else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
    {
        fullPath    = x_BasePath + "res/Shaders/Compiled/DXIL/" + (shaderFilename + ".dxil");
        format      = SDL_GPU_SHADERFORMAT_DXIL;
        formatFound = true;
    }
    assert(formatFound);
    assert(std::filesystem::exists(fullPath));

    std::ifstream file {fullPath, std::ios::binary};
    assert(file);  // check if we successfully opened the file

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
    assert(shader != nullptr);

    return shader;
}

SDL_Surface *LoadImage(const std::string &imageFileName, int desiredChannels)
{
    static bool x_BasePathFound = false;
    static std::string x_BasePath {};

    if (!x_BasePathFound)
    {
        x_BasePath      = SDL_GetBasePath();
        x_BasePathFound = true;
    }

    std::string fullPath {};
    SDL_Surface *result;
    SDL_PixelFormat format;

    fullPath = x_BasePath + "res/Images/" + (imageFileName + ".bmp");

    result = SDL_LoadBMP(fullPath.c_str());
    assert(result != nullptr);

    if (desiredChannels == 4) { format = SDL_PIXELFORMAT_ABGR8888; }
    else
    {
        SDL_DestroySurface(result);
        return nullptr;
    }
    if (result->format != format)
    {
        SDL_Surface *next = SDL_ConvertSurface(result, format);
        SDL_DestroySurface(result);
        result = next;
    }

    return result;
}

}  // namespace Common
