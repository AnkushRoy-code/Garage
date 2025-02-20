#ifndef INCLUDE_COMMON_COMMON_H_
#define INCLUDE_COMMON_COMMON_H_

#include <SDL3/SDL_gpu.h>
#include <string>

namespace Common
{
/**
* This is a function that returns a compiled shader after taking the path to shader.
*
* @return SDL_GPUShader a Uint32 that points to the compiled shader in the GPU.
*
* @code
*
* // a simple vertex shader example
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
}  // namespace Common

#endif  // INCLUDE_COMMON_COMMON_H_
