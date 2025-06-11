#include "Core/Common/pch.h"

#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "Core/Console.h"
#include "Core/Event/EventHandler.h"

#include "Projects/N_Body_Simulation/src/Main.h"
#include "Projects/N_Body_Simulation/src/Particle.h"

void N_Body_Simulation::InitialiseTransferBuffers()
{
    const SDL_GPUTransferBufferCreateInfo tBufCreateInfo {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),
    };

    auto &rndt       = Core::Context::GetContext()->RenderData;
    m_TransferBuffer = SDL_CreateGPUTransferBuffer(rndt.Device, &tBufCreateInfo);

    const SDL_GPUBufferCreateInfo newBufCreateInfo {
        .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
        .size  = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),

    };

    m_DataBuffer = SDL_CreateGPUBuffer(rndt.Device, &newBufCreateInfo);
}

bool N_Body_Simulation::Init()
{
    hasUI = true;

    auto &rndt = Core::Context::GetContext()->RenderData;
    // pipeline creation
    SDL_GPUShader *vertShader = Common::LoadShader(rndt.Device, "DrawCircle.vert", 0, 1, 1, 0);
    SDL_GPUShader *fragShader = Common::LoadShader(rndt.Device, "DrawCircle.frag", 0, 0, 0, 0);

    const SDL_GPUColorTargetBlendState blendState {
        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .color_blend_op        = SDL_GPU_BLENDOP_ADD,
        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
        .enable_blend          = true,
    };

    const SDL_GPUColorTargetDescription colorDesc {
        .format      = SDL_GetGPUSwapchainTextureFormat(rndt.Device, rndt.Window),
        .blend_state = blendState,
    };

    const SDL_GPUGraphicsPipelineTargetInfo targetInfo {
        .color_target_descriptions = &colorDesc,
        .num_color_targets         = 1,
    };

    const SDL_GPUMultisampleState mState = {
        .sample_count = rndt.SampleCount,  // MSAA or not
    };

    const SDL_GPUGraphicsPipelineCreateInfo createInfo {
        .vertex_shader     = vertShader,
        .fragment_shader   = fragShader,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
        .multisample_state = mState,
        .target_info       = targetInfo,
    };

    m_RenderPipeline = SDL_CreateGPUGraphicsPipeline(rndt.Device, &createInfo);

    SDL_ReleaseGPUShader(rndt.Device, vertShader);
    SDL_ReleaseGPUShader(rndt.Device, fragShader);

    InitialiseTransferBuffers();

    m_Particles.Init();

    const auto w = Core::Context::GetContext()->AppState.ProjectWindowSize.x;
    const auto h = Core::Context::GetContext()->AppState.ProjectWindowSize.y;

    m_Camera = Camera(glm::vec3(w / 2.0f, h / 2.0f, 700.0f));

    static bool firstTime = true;
    if (firstTime)
    {
        Core::ConsoleLogBuffer::AddMessage(
            "N-Body-Simulation is initialised\n"
            "In this there are 4 different arrangements you can choose from to view.\n"
            "It also has camera support! Just try clicking the project window or the Lock mouse "
            "checkbox. You can exit out of camera mode by pressing ESC have fun!");
        firstTime = false;
    }

    return true;
}

bool N_Body_Simulation::Update()
{
    m_Particles.Update();
    m_Camera.ProcessEvents();
    return true;
}

bool N_Body_Simulation::Draw()
{
    auto &rndt = Core::Context::GetContext()->RenderData;
    auto &apst = Core::Context::GetContext()->AppState;

    const float w = apst.ProjectWindowSize.x;
    const float h = apst.ProjectWindowSize.y;

    // const glm::mat4x4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
    // const glm::mat4x4 finalProjection = projection * m_Camera.GetViewMatrix();

    float fov       = glm::radians(45.0f);
    float aspect    = w / h;
    float nearPlane = 0.1f;
    float farPlane  = 100.0f;

    glm::mat4 perspectiveProjection = glm::perspective(fov, aspect, nearPlane, farPlane);

    glm::mat4 finalProjection = perspectiveProjection * m_Camera.GetViewMatrix();

    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(rndt.Device);
    assert(cmdBuf);

    assert(rndt.ProjectTexture);

    // upload the data to transfer buffer
    auto *dataPtr = static_cast<ParticleDataSend *>(
        SDL_MapGPUTransferBuffer(rndt.Device, m_TransferBuffer, true));

    for (int i = 0; i < ParticleContainer::count; i++)
    {
        dataPtr[i].Color    = m_Particles.ParticleVec[i].Color;
        dataPtr[i].Position = m_Particles.ParticleVec[i].Position;
        dataPtr[i].Radius   = m_Particles.ParticleVec[i].Radius;
        dataPtr[i]._padding = 0.0f;
    }

    SDL_UnmapGPUTransferBuffer(rndt.Device, m_TransferBuffer);

    // upload the data to gpu from transfer buffers
    const SDL_GPUTransferBufferLocation transferBufferLocation {
        .transfer_buffer = m_TransferBuffer,
        .offset          = 0,
    };

    const SDL_GPUBufferRegion bufferRegion {
        .buffer = m_DataBuffer,
        .offset = 0,
        .size   = (Uint32)(ParticleContainer::count * sizeof(ParticleDataSend)),
    };

    SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(cmdBuf);
    SDL_UploadToGPUBuffer(copyPass, &transferBufferLocation, &bufferRegion, true);
    SDL_EndGPUCopyPass(copyPass);

    SDL_GPUColorTargetInfo tinfo {.texture     = rndt.ProjectTexture,
                                  .clear_color = {0.094f, 0.094f, 0.145f, 1.00f},
                                  .load_op     = SDL_GPU_LOADOP_CLEAR,
                                  .store_op    = SDL_GPU_STOREOP_STORE,
                                  .cycle       = false};

    if (rndt.SampleCount == SDL_GPU_SAMPLECOUNT_1) { tinfo.store_op = SDL_GPU_STOREOP_STORE; }
    else
    {
        tinfo.store_op        = SDL_GPU_STOREOP_RESOLVE;
        tinfo.resolve_texture = rndt.ResolveTexture;
    }

    rndt.ProjectPass = SDL_BeginGPURenderPass(cmdBuf, &tinfo, 1, nullptr);

    SDL_BindGPUGraphicsPipeline(rndt.ProjectPass, m_RenderPipeline);
    SDL_BindGPUVertexStorageBuffers(rndt.ProjectPass, 0, &m_DataBuffer, 1);

    SDL_PushGPUVertexUniformData(cmdBuf, 0, &finalProjection, sizeof(glm::mat4x4));

    SDL_DrawGPUPrimitives(rndt.ProjectPass, 4, ParticleContainer::count, 0, 0);

    SDL_EndGPURenderPass(rndt.ProjectPass);

    SDL_GPUTexture *blitSourceTexture =
        (tinfo.resolve_texture != nullptr) ? tinfo.resolve_texture : tinfo.texture;

    const SDL_GPUBlitRegion blitSrc = {
        .texture = blitSourceTexture,
        .w       = (Uint32)apst.ProjectWindowSize.x,
        .h       = (Uint32)apst.ProjectWindowSize.y,
    };

    SDL_BindGPUFragmentStorageTextures(rndt.ProjectPass, 1, &rndt.ProjectTexture, 2);

    const SDL_GPUBlitRegion blitDst = {
        .texture = rndt.ProjectTexture,
        .w       = (Uint32)apst.ProjectWindowSize.x,
        .h       = (Uint32)apst.ProjectWindowSize.y,
    };

    const SDL_GPUBlitInfo blitInfo = {.source      = blitSrc,
                                      .destination = blitDst,
                                      .load_op     = SDL_GPU_LOADOP_DONT_CARE,
                                      .filter      = SDL_GPU_FILTER_LINEAR};

    SDL_BlitGPUTexture(cmdBuf, &blitInfo);

    SDL_SubmitGPUCommandBuffer(cmdBuf);

    return true;
}

void N_Body_Simulation::Quit()
{
    m_Particles.Quit();
}

bool N_Body_Simulation::DrawUI()
{
    if (ImGui::Begin("N_Body_Simulation Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");

        static const std::string names[4] = {"Two Bodies", "Four Bodies", "Circular arrangement",
                                             "Grid Arrangement"};

        static unsigned int index = 0;

        if (ImGui::BeginCombo("Simulation", names[index].c_str()))
        {
            for (int i = 0; i < m_Particles.ParticlesData.size(); i++)
            {
                const bool isSelected = (index == i);
                if (ImGui::Selectable(names[i].c_str(), isSelected))
                {
                    if (i != index)
                    {
                        index = i;
                        m_Particles.InitData(index);
                        InitialiseTransferBuffers();
                    }
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Restart Simulation")) { m_Particles.InitData(index); }

        if (ImGui::Checkbox("Lock Mouse", &m_Camera.lockMouse))
        {
            SDL_SetWindowRelativeMouseMode(Core::Context::GetContext()->RenderData.Window,
                                           m_Camera.lockMouse);
        }

        static bool yeah = false;
        if (ImGui::Checkbox("Locks the mouse inside the window", &yeah))
        {
            SDL_SetWindowMouseGrab(Core::Context::GetContext()->RenderData.Window, yeah);
        }
    }
    ImGui::End();

    return true;
}
