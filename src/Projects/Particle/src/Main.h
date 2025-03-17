#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Common/BaseProject.h"

class Particle : public BaseProject
{
    bool Init() override;
    bool Update() override;
    bool Draw() override;
    void Quit() override;
};

#endif  // INCLUDE_SRC_MAIN_H_
