#ifndef INCLUDE_SRC_PARTICLE_H_
#define INCLUDE_SRC_PARTICLE_H_

#include <array>

enum ParticleColour
{
    RED,
    GREEN,
    BLUE,
    WHITE,
    YELLOW,
    PURPLE,
    CYAN,
    MAGENTA,
    COLOR_COUNT
};

struct ParticleData
{
    int Radius              = 3;
    int ParticleCount       = 100;
    int NumOfParticleColor  = 4;
    int ImGuiWindowWidth    = 360;
    int SameMinDist         = 5;
    int SameMaxDist         = 250;
    bool ShowMinDistControl = false;
    bool ShowMaxDistControl = false;
    bool ShowSameMinDist    = false;
    bool ShowSameMaxDist    = false;

    std::array<float, COLOR_COUNT * COLOR_COUNT> Force {};
    std::array<int, COLOR_COUNT * COLOR_COUNT> MinDist {};
    std::array<int, COLOR_COUNT * COLOR_COUNT> MaxDist {};
};

extern ParticleData gParticleData;

#endif  // INCLUDE_SRC_PARTICLE_H_
