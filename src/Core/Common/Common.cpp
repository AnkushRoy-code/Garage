#include "Core/Common/Common.h"
#include "config.h"

namespace Common
{

// returns the resource directory
const std::filesystem::path &GetBasePath()
{
    static bool firstTime             = false;
    static std::filesystem::path base = {DATA_DIR};

    if (!firstTime)
    {
        // clang-format off
        const std::filesystem::path bases[] {
            "src/Main",
            DATA_DIR,
            DATA_DIR_LOCAL,
            SDL_GetBasePath()
        };
        // clang-format on

        for (const auto &i: bases)
        {
            if (std::filesystem::exists(i / "res"))
            {
                base = i;
                return base;
            }
        }

        firstTime = true;
    }

    return base;
}

SDL_GPUShader *LoadShader(SDL_GPUDevice *device,
                          const std::string &shaderFilename,
                          Uint32 samplerCount,
                          Uint32 uniformBufferCount,
                          Uint32 storageBufferCount,
                          Uint32 storageTextureCount)
{
    // Determine shader stage
    SDL_GPUShaderStage stage;
    if (shaderFilename.rfind(".vert") != std::string::npos) { stage = SDL_GPU_SHADERSTAGE_VERTEX; }
    else { stage = SDL_GPU_SHADERSTAGE_FRAGMENT; }

    // Choose compiled format
    SDL_GPUShaderFormat format {};
    std::string entrypoint {"main"};
    const auto backend               = SDL_GetGPUShaderFormats(device);
    std::filesystem::path shaderPath = GetBasePath() / "res" / "Shaders" / "Compiled";

    if (backend & SDL_GPU_SHADERFORMAT_SPIRV)
    {
        shaderPath /= "SPIRV";
        shaderPath /= shaderFilename + ".spv";
        format = SDL_GPU_SHADERFORMAT_SPIRV;
    }
    else if (backend & SDL_GPU_SHADERFORMAT_MSL)
    {
        shaderPath /= "MSL";
        shaderPath /= shaderFilename + ".msl";
        format     = SDL_GPU_SHADERFORMAT_MSL;
        entrypoint = "main0";
    }
    else if (backend & SDL_GPU_SHADERFORMAT_DXIL)
    {
        shaderPath /= "DXIL";
        shaderPath /= shaderFilename + ".dxil";
        format = SDL_GPU_SHADERFORMAT_DXIL;
    }
    else { assert(false && "Unsupported shader format"); }

    assert(std::filesystem::exists(shaderPath) && "Shader file not found");

    std::ifstream file {shaderPath, std::ios::binary};

    assert(file && "Failed to open shader file");

    std::vector<Uint8> code {std::istreambuf_iterator<char>(file), {}};

    SDL_GPUShaderCreateInfo info {
        .code_size            = code.size(),
        .code                 = code.data(),
        .entrypoint           = entrypoint.c_str(),
        .format               = format,
        .stage                = stage,
        .num_samplers         = samplerCount,
        .num_storage_textures = storageTextureCount,
        .num_storage_buffers  = storageBufferCount,
        .num_uniform_buffers  = uniformBufferCount,
    };

    SDL_GPUShader *shader = SDL_CreateGPUShader(device, &info);
    assert(shader && "Shader creation failed");

    return shader;
}

SDL_Surface *LoadImage(const std::string &imageFileName, int desiredChannels)
{
    std::filesystem::path fullPath = GetBasePath() / "res" / "Images" / (imageFileName + ".bmp");

    SDL_Surface *result;
    SDL_PixelFormat format;

    result = SDL_LoadBMP(fullPath.string().c_str());

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
