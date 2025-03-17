#include "ImGuiMenu.h"

#include "Particle.h"
#include "ParticleController.h"
#include "imgui.h"

#include <imgui_internal.h>

#include <string>
#include <array>

// using namespace ImGuiPrivate;  // just pray no one includes a cpp file

void HelpMarker(const std::string &str)
{
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(str.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGuiParticleWindow()
{
    ImGuiPrivate::WindowMain();
}

void ImGuiPrivate::WindowMain()
{
    if (ImGui::Begin("Control Panel"))
    {
        ShowGlobalVariables();

        ShowForce();
        if (gParticleData.ShowMinDistControl) { ShowMinDist(); }
        if (gParticleData.ShowMaxDistControl) { ShowMaxDist(); }
        ImGui::End();
    }
}

void ImGuiPrivate::ShowGlobalVariables()
{

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                io.Framerate);

    auto m_ImGuiWindowWidth = ImGui::GetWindowWidth();

    ImGui::SeparatorText("Global variables");

    ImGui::SliderInt("Radius", &gParticleData.Radius, 1, 10, nullptr, ImGuiSliderFlags_AlwaysClamp);

    ImGui::PushID(1);
    ImGui::SliderInt("Particle Count", &gParticleData.ParticleCount, 10, 1000, nullptr,
                     ImGuiSliderFlags_AlwaysClamp);
    {
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            ParticleController::initializeParticle(gParticleData.ParticleCount,
                                                   gParticleData.NumOfParticleColor);
        }
    };
    ImGui::PopID();
    HelpMarker("This changes the number of particles for each colour");

    (ImGui::SliderInt("Types of Colour", &gParticleData.NumOfParticleColor, 2, 8, nullptr,
                      ImGuiSliderFlags_AlwaysClamp));
    {
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            ParticleController::initializeParticle(gParticleData.ParticleCount,
                                                   gParticleData.NumOfParticleColor);
        }
    }
    HelpMarker("This changes the number of different colours in the simulation");

    if (ImGui::Button("Refresh"))
    {
        ParticleController::initializeParticle(gParticleData.ParticleCount,
                                               gParticleData.NumOfParticleColor);
    }

    if (!gParticleData.ShowSameMinDist)
    {
        ImGui::Checkbox("Show MinDist Control", &gParticleData.ShowMinDistControl);
        if (!gParticleData.ShowMinDistControl) { ImGui::SameLine(); }
    }

    if (!gParticleData.ShowMinDistControl)
    {
        ImGui::Checkbox("Same Min Dist?##3", &gParticleData.ShowSameMinDist);
    }

    if (gParticleData.ShowSameMinDist)
    {
        if (ImGui::SliderInt("Minimum dist", &gParticleData.SameMinDist, 1, 30, nullptr,
                             ImGuiSliderFlags_AlwaysClamp))
        {
            ParticleController::changeAllMin(gParticleData.SameMinDist);
        }
    }

    if (!gParticleData.ShowSameMaxDist)
    {
        ImGui::Checkbox("Show MaxDist Control", &gParticleData.ShowMaxDistControl);
        if (!gParticleData.ShowMaxDistControl) { ImGui::SameLine(); }
    }
    if (!gParticleData.ShowMaxDistControl)
    {
        ImGui::Checkbox("Same Max Dist?##3", &gParticleData.ShowSameMaxDist);
    }

    if (gParticleData.ShowSameMaxDist)
    {
        if (ImGui::SliderInt("Maximum dist", &gParticleData.SameMaxDist, 150, 300, nullptr,
                             ImGuiSliderFlags_AlwaysClamp))
        {
            ParticleController::changeAllMax(gParticleData.SameMaxDist);
        }
    }
}

void ImGuiPrivate::ShowMaxDist()
{

    if (ImGui::CollapsingHeader("Maximum Distance Control"))
    {

        if (ImGui::Button("Random Max Dist")) { ParticleController::populateRandomMaxDistance(); }

        ImGui::SameLine();

        if (ImGui::Button("Minimise Max Dist")) { ParticleController::minimiseMaxDistance(); }
        if (ImGui::Button("Maximise Max Distance")) { ParticleController::maximiseMaxDistance(); }
        if (ImGui::Button("Default MaxDist")) { ParticleController::defaultMaxDistance(); }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Save Max Dist To File"))
        {
            ImGui::LogToFile(2, "maxDist.txt");
            ImGui::SameLine();
            HelpMarker("These forces is saved inside maxDist.txt file");
        }

        ShowMaxDistSliders();
        ImGui::LogFinish();
    }
}

void ImGuiPrivate::ShowMinDist()
{
    if (ImGui::CollapsingHeader("Minimum Distance Control"))
    {
        if (ImGui::Button("Random Min Dist")) { ParticleController::populateRandomMinDistance(); }

        ImGui::SameLine();

        if (ImGui::Button("Minimise Min Dist")) { ParticleController::minimiseMinDistance(); }
        if (ImGui::Button("Maximise Min Distance")) { ParticleController::maximiseMinDistance(); }
        if (ImGui::Button("Default MinDist")) { ParticleController::defaultMinDistance(); }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Save Min Dist To File"))
        {
            ImGui::LogToFile(2, "minDist.txt");
            ImGui::SameLine();
            HelpMarker("These forces is saved inside minDist.txt file");
        }

        ShowMinDistSliders();
        ImGui::LogFinish();
    }
}

void ImGuiPrivate::ShowMaxDistSliders()
{
    createMaxDistTreeNode("RedMax", RED);
    createMaxDistTreeNode("GreenMax", GREEN);
    createMaxDistTreeNode("BlueMax", BLUE);
    createMaxDistTreeNode("WhiteMax", WHITE);
    createMaxDistTreeNode("YellowMax", YELLOW);
    createMaxDistTreeNode("PurpleMax", PURPLE);
    createMaxDistTreeNode("CyanMax", CYAN);
    createMaxDistTreeNode("MagentaMax", MAGENTA);
}

void ImGuiPrivate::ShowMinDistSliders()
{
    createMinDistTreeNode("RedMin", RED);
    createMinDistTreeNode("GreenMin", GREEN);
    createMinDistTreeNode("BlueMin", BLUE);
    createMinDistTreeNode("WhiteMin", WHITE);
    createMinDistTreeNode("YellowMin", YELLOW);
    createMinDistTreeNode("PurpleMin", PURPLE);
    createMinDistTreeNode("CyanMin", CYAN);
    createMinDistTreeNode("MagentaMin", MAGENTA);
}

void ImGuiPrivate::createMinDistTreeNode(const std::string &label, int colorIndex)
{
    createTreeNodeWithSliders(
        label, colorIndex,
        [](const std::string &sliderLabel, int colorIndex, int otherColor)
        {
            if (otherColor == colorIndex)
            {
                ImGui::SliderInt(sliderLabel.c_str(),
                                 &gParticleData.MinDist[colorIndex][otherColor], 1, 30, "%d",
                                 ImGuiSliderFlags_AlwaysClamp);
            }
            else
            {
                checkBoolMinDist(colorIndex, static_cast<ParticleColour>(otherColor), sliderLabel);
            }
        });
}

void ImGuiPrivate::createMaxDistTreeNode(const std::string &label, int colorIndex)
{
    createTreeNodeWithSliders(
        label, colorIndex,
        [](const std::string &sliderLabel, int colorIndex, int otherColor)
        {
            if (otherColor == colorIndex)
            {
                ImGui::SliderInt(sliderLabel.c_str(),
                                 &gParticleData.MaxDist[colorIndex][otherColor], 150, 300, "%d",
                                 ImGuiSliderFlags_AlwaysClamp);
            }
            else
            {
                checkBoolMaxDist(colorIndex, static_cast<ParticleColour>(otherColor), sliderLabel);
            }
        });
}

void ImGuiPrivate::createTreeNodeWithSliders(
    const std::string &label,
    int colorIndex,
    const std::function<void(const std::string &, int, int)> &sliderFunc)
{
    if (gParticleData.NumOfParticleColor < colorIndex + 1) return;

    if (!ImGui::TreeNode(label.c_str())) return;

    for (int otherColor = 0; otherColor < COLOR_COUNT; ++otherColor)
    {
        std::string sliderLabel = std::string(label).substr(0, 1) + " x " + "RGBWYPCM"[otherColor]
                                  + "##" + std::to_string(colorIndex) + std::to_string(otherColor);

        sliderFunc(sliderLabel, colorIndex, otherColor);
    }

    ImGui::TreePop();
    ImGui::Spacing();
}

void ImGuiPrivate::checkBoolMinDist(int start, int end, const std::string &string)
{
    if (gParticleData.NumOfParticleColor >= end + 1)
    {
        ImGui::SliderInt(string.c_str(), &gParticleData.MinDist[start][end], 1, 30, "%d",
                         ImGuiSliderFlags_AlwaysClamp);
    }
}

void ImGuiPrivate::checkBoolMaxDist(int start, int end, const std::string &string)
{
    if (gParticleData.NumOfParticleColor >= end + 1)
    {
        ImGui::SliderInt(string.c_str(), &gParticleData.MaxDist[start][end], 150, 300, "%d",
                         ImGuiSliderFlags_AlwaysClamp);
    }
}

// That was all min/max dist now its about Force.

void ImGuiPrivate::ShowForce()
{
    if (ImGui::CollapsingHeader("Force Control"))
    {
        if (ImGui::Button("Random Forces")) { ParticleController::populateRandomForce(); }

        ImGui::SameLine();

        if (ImGui::Button("Reset Force")) { ParticleController::resetForce(); }

        ImGui::SameLine();

        if (ImGui::Button("Minimise Force")) { ParticleController::minimiseForce(); }
        if (ImGui::Button("Maximise Force")) { ParticleController::maximiseForce(); }

        ImGui::SameLine();

        if (ImGui::Button("Default Force")) { ParticleController::defaultForce(); }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Save Forces To File"))
        {
            ImGui::LogToFile(2, "forces.txt");

            // FIX: This helper line doesn't show fsr and the ones later on like
            // this
            ImGui::SameLine();
            HelpMarker("These forces is saved inside forces.txt file");
        }
        ShowColorSliders();
        ImGui::LogFinish();
    }
}

void ImGuiPrivate::ShowColorSliders()
{
    createColorTreeNode("Red", RED);
    createColorTreeNode("Green", GREEN);
    createColorTreeNode("Blue", BLUE);
    createColorTreeNode("White", WHITE);
    createColorTreeNode("Yellow", YELLOW);
    createColorTreeNode("Purple", PURPLE);
    createColorTreeNode("Cyan", CYAN);
    createColorTreeNode("Magenta", MAGENTA);
}

void ImGuiPrivate::createColorTreeNode(const std::string &label, int colorIndex)
{
    createTreeNodeWithSliders(
        label, colorIndex,
        [](const std::string &sliderLabel, int colorIndex, int otherColor)
        {
            if (otherColor == colorIndex)
            {
                ImGui::SliderFloat(sliderLabel.c_str(),
                                   &gParticleData.Force[colorIndex][otherColor], -1, 1, "%.2f",
                                   ImGuiSliderFlags_AlwaysClamp);
            }
            else { checkBool(colorIndex, static_cast<ParticleColour>(otherColor), sliderLabel); }
        });
}

void ImGuiPrivate::checkBool(int start, int end, const std::string &string)
{
    if (gParticleData.NumOfParticleColor >= end + 1)
    {
        ImGui::SliderFloat(string.c_str(), &gParticleData.Force[start][end], -1, 1, "%.3f",
                           ImGuiSliderFlags_AlwaysClamp);
    }
}
