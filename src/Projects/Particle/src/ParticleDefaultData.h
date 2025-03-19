#include "Projects/Particle/src/ParticleData.h"
#include "SDL3/SDL_pixels.h"
#include <array>

// getting into the habit of inline globals
namespace ParticleDefaultData
{
inline constexpr std::array<float, COLOR_COUNT * COLOR_COUNT> m_defaultForceValue {{
    // clang-format off
    //RED      GREEN    BLUE     WHITE    YELLOW   PURPLE  CYAN      MAGENTA
     0.10f,    0.426f, -0.390f,  0.537f, -0.728f,  0.372f, 0.945f,  -0.535f,  // RED,
     0.233f,  -0.87f,  -0.142f, -0.152f,  0.810f, -0.973f, 0.255f,  -0.645f,  // GREEN,
     0.317f,  -0.724f, -0.09f,   0.987f, -0.233f,  0.593f, 0.601f,  -0.178f,  // BLUE,
     0.709f,  -0.114f,  0.103f, -0.28f,   0.112f, -0.952f, -0.119f,  0.720f,  // WHITE,
     0.452f,  -0.831f,  0.241f, -0.678f,  0.993f, -0.054f, 0.515f,  -0.407f,  // YELLOW,
     -0.323f,  0.768f, -0.563f,  0.927f, -0.395f,  0.216f, -0.489f,  0.340f,  // PURPLE,
     0.615f,  -0.129f,  0.853f, -0.577f,  0.434f, -0.762f, 0.261f,  -0.368f,  // CYAN,
     0.287f,  -0.571f,  0.172f,  0.686f, -0.412f,  0.745f, -0.932f,  0.073f   // MAGENTA
    // clang-format on
}};

inline constexpr std::array<int, COLOR_COUNT * COLOR_COUNT> m_defaultMinDistanceValue {{
    // clang-format off
  //RED GREEN BLUE WHITE YELLOW PURPLE CYAN MAGENTA
    20,  20,   20,  20,   21,    27,    12,  8  ,   // RED,
    20,  20,   20,  20,   26,    15,    24,  9  ,   // GREEN,
    20,  20,   20,  20,    4,    29,    11,  23 ,  // BLUE,
    20,  20,   20,  20,   14,    20,    30,  28 ,  // WHITE,
    1 ,  22,   25,   6,    3,    19,    10,  13 ,  // YELLOW,
    30,  2,    5,   16,    8,    18,    14,  7  ,   // PURPLE,
    11,  28,   23,   4,    9,    21,    17,  6  ,   // CYAN,
    27,  12,   29,   1,   26,     3,    15,  24 // MAGENTA
    // clang-format on
}};

inline constexpr std::array<int, COLOR_COUNT * COLOR_COUNT> m_defaultMaxDistanceValue {{
    // clang-format off
   //RED   GREEN BLUE  WHITE YELLOW PURPLE CYAN  MAGENTA
     150,  150,  150,  150,  217,   259,   178,  294,  // RED,
     150,  150,  150,  150,  239,   188,   195,  284,  // GREEN,
     150,  150,  150,  150,  258,   293,   196,  263,  // BLUE,
     150,  150,  150,  150,  201,   176,   269,  185,  // WHITE,
     245,  289,  270,  186,  241,   200,   221,  198,  // YELLOW,
     211,  179,  258,  272,  290,   184,   237,  205,  // PURPLE,
     294,  196,  201,  248,  176,   284,   239,  170,  // CYAN,
     169,  265,  288,  154,  275,   217,   230,  283   // MAGENTA
    // clang-format on
}};

inline constexpr std::array<SDL_Color, COLOR_COUNT> ColorMap {{
    {255, 0, 0, 255},      // RED
    {0, 255, 0, 255},      // GREEN
    {0, 0, 255, 255},      // BLUE
    {255, 255, 255, 255},  // WHITE
    {255, 255, 25, 255},   // YELLOW
    {145, 30, 180, 255},   // PURPLE
    {70, 240, 240, 255},   // CYAN
    {240, 50, 230, 255}    // MAGENTA
}};
}  // namespace ParticleDefaultData
