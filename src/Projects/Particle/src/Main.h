#ifndef INCLUDE_SRC_MAIN_H_
#define INCLUDE_SRC_MAIN_H_

#include "Common/BaseProject.h"

class Particle : public BaseProject
{
    bool Init(Context &context) override;
    bool Update(Context &context) override;
    bool Draw(Context &context) override;
    void Quit(Context &context) override;
};

#endif  // INCLUDE_SRC_MAIN_H_
