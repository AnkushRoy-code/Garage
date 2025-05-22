#ifndef INCLUDE_COMMON_COMMON_H_
#define INCLUDE_COMMON_COMMON_H_

#include "Core/Common/pch.h"

#ifdef TRACY_ENABLE
    #include <tracy/Tracy.hpp>
    #define PROFILE_SCOPE         ZoneScoped
    #define PROFILE_SCOPE_N(name) ZoneScopedN(name)
    #define PROFILE_FRAME         FrameMark
#else
    #define PROFILE_SCOPE
    #define PROFILE_SCOPE_N(name)
    #define PROFILE_FRAME
#endif

namespace Common
{
/**
 * This is a function that returns a compiled shader after taking the path to shader.
 *
 * @return SDL_GPUShader a Uint32 that points to the compiled shader in the GPU.
 *
 * @code
 *
 * // a simple vertex shader project
 *
 * auto vertexShader = LoadShader(SDL_GPUDevice, "shader.vert", 0, 0, 0, 0);
 *
 * @endcode
 */
SDL_GPUShader *LoadShader(SDL_GPUDevice *device,
                          const std::string &shaderFilename,
                          Uint32 samplerCount,
                          Uint32 uniformBufferCount,
                          Uint32 storageBufferCount,
                          Uint32 storageTextureCount);

/// @warning ONLY BMP IMAGES SUPPORTED BECAUSE I AM LAZY AND DIDN'T LOOK INTO OTHER FORMATS
SDL_Surface *LoadImage(const std::string &imageFileName, int desiredChannels = 4);
}  // namespace Common

#endif  // INCLUDE_COMMON_COMMON_H_
