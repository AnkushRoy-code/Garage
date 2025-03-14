#ifndef INCLUDE_SRC_IMGUIMENU_H_
#define INCLUDE_SRC_IMGUIMENU_H_

#include "imgui.h"
#include <functional>
#include <string>

/// The main particle window is initiated by this function
void ImGuiParticleWindow();

/// The user is not supposed to use this functions in main code
namespace ImGuiPrivate
{
void WindowMain(ImGuiWindowFlags WinFlags);

// Show::What
void ShowForce();  // left
void ShowGlobalVariables();
void ShowMaxDist();
void ShowMinDist();

// Show::What::Sliders
void ShowMaxDistSliders();
void ShowMinDistSliders();
void ShowColorSliders();

void createMinDistTreeNode(const std::string &label, int colorIndex);
void createMaxDistTreeNode(const std::string &label, int colorIndex);
void createColorTreeNode(const std::string &label, int colorIndex);

void createTreeNodeWithSliders(
    const std::string &label,
    int colorIndex,
    const std::function<void(const std::string &, int, int)> &sliderFunc);

void checkBoolMinDist(int start, int end, const std::string &string);
void checkBoolMaxDist(int start, int end, const std::string &string);
void checkBool(int start, int end, const std::string &string);

}  // namespace ImGuiPrivate

#endif  // INCLUDE_SRC_IMGUIMENU_H_
