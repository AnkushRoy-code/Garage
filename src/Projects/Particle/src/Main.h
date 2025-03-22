#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Common/BaseProject.h"

class Particle : public Common::BaseProject, public Common::ImGuiUI
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;

    std::string name = "Particle";
    [[nodiscard]] const std::string &getName() override {
        return name;
    }

    bool DrawUI() override;
};

#endif  // INCLUDE_SRC_MAIN_H_
