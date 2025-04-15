#include "Projects/Cube/Main.h"
#include "Core/Common/Common.h"
#include "Core/Context.h"
#include "SDL3/SDL_gpu.h"

bool Cube::Init()
{
    auto &rndt = Core::Context::GetContext()->RenderData;

    SDL_GPUShader *vertShader = Common::LoadShader(rndt.Device, "Grid.vert", 0, 0, 0, 0);
    assert(vertShader);

    SDL_GPUShader *fragShader = Common::LoadShader(rndt.Device, "Grid.frag", 0, 0, 0, 0);
    assert(fragShader);

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

    const SDL_GPUGraphicsPipelineTargetInfo tinfo = {
        .color_target_descriptions = &colorDesc,
        .num_color_targets         = 1,
    };

    const SDL_GPUMultisampleState state = {
        .sample_count = rndt.SampleCount,
    };

    const SDL_GPUGraphicsPipelineCreateInfo info = {
        .vertex_shader     = vertShader,
        .fragment_shader   = fragShader,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .multisample_state = state,
        .target_info       = tinfo,
    };

    m_RenderPipeline = SDL_CreateGPUGraphicsPipeline(rndt.Device, &info);
    assert(!m_RenderPipeline);

    SDL_ReleaseGPUShader(rndt.Device, vertShader);
    SDL_ReleaseGPUShader(rndt.Device, fragShader);

    const auto w = Core::Context::GetContext()->AppState.ProjectWindowSize.x;
    const auto h = Core::Context::GetContext()->AppState.ProjectWindowSize.y;

    m_Camera = Camera(glm::vec3(w / 2.0f, h / 2.0f, 700.0f));

    return true;
}

bool Cube::Update()
{
    m_Camera.ProcessEvents();
    return true;
}

bool Cube::Draw()
{

    return true;
}

void Cube::Quit()
{
    //
}

bool Cube::DrawUI()
{
    if (ImGui::Begin("N_Body_Simulation Controller###ProjectUI"))
    {
        ImGui::SeparatorText("Options");
    }
    ImGui::End();

    return true;
}
