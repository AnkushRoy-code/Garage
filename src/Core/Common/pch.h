#ifndef INCLUDE_PCH_PCH_H_
#define INCLUDE_PCH_PCH_H_

// #if defined(USE_IMGUI) || defined(USE_IMGUI_BACKENDS)
//     #include <imgui.h>
//     #ifdef USE_IMGUI_BACKENDS
//         #include <backends/imgui_impl_sdl3.h>
//         #include <backends/imgui_impl_sdlgpu3.h>
//     #endif  // USE_IMGUI_BACKENDS
// #endif  // USE_IMGUI

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlgpu3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_scancode.h>

#include <glm/glm.hpp>
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <exception>
#include <cmath>


#endif  // INCLUDE_PCH_PCH_H_
