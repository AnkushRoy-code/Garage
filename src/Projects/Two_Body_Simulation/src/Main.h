#ifndef INCLUDE_2_BODY_SIMULATION_MAIN_H_
#define INCLUDE_2_BODY_SIMULATION_MAIN_H_

#include "Projects/Common/BaseProject.h"
#include "Particle.h"
#include <SDL3/SDL_gpu.h>

class Two_Body_Simulation : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    bool DrawUI() override;

    std::string name = "2-Body-Simulation";
    [[nodiscard]] const std::string &getName() override
    {
        return name;
    }

    SDL_GPUGraphicsPipeline *renderPipeline;
    SDL_GPUTransferBuffer *transferBuffer;
    SDL_GPUBuffer *dataBuffer;

    ParticleContainer Particles {};
};

#endif  // INCLUDE_2_BODY_SIMULATION_MAIN_H_
