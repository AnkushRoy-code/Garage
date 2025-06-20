#ifndef INCLUDE_2_BODY_SIMULATION_MAIN_H_
#define INCLUDE_2_BODY_SIMULATION_MAIN_H_

#include "Projects/Common/BaseProject.h"
#include "Projects/Common/Camera.h"
#include "Projects/N_Body_Simulation/src/Particle.h"

#include <SDL3/SDL_gpu.h>

class N_Body_Simulation : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    bool DrawUI() override;

    std::string name = "N-Body-Simulation";
    [[nodiscard]] const std::string &GetName() override
    {
        return name;
    }

    SDL_GPUGraphicsPipeline *m_RenderPipeline;
    SDL_GPUTransferBuffer *m_TransferBuffer;
    SDL_GPUBuffer *m_DataBuffer;

    ParticleContainer m_Particles {};
    Camera m_Camera {};

    void InitialiseTransferBuffers();
};

#endif  // INCLUDE_2_BODY_SIMULATION_MAIN_H_
