#include "Flags.h"
#include "SDL3/SDL_init.h"

#include <cxxopts.hpp>

bool HandleFLags::HandleFlags(int argc, char *argv[])
{
    cxxopts::Options options("Garage", "A collection of my Creations!!");

    // clang-format off
    options.add_options()
        ("b,bar", "Param bar", cxxopts::value<std::string>())
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
        ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
        ("h,help", "Print usage")
    ;  // clang-format on

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        SDL_Init(SDL_INIT_VIDEO);
        return false;
    }

    bool debug = result["debug"].as<bool>();
    std::string bar;
    if (result.count("bar"))
        bar = result["bar"].as<std::string>();
    int foo = result["foo"].as<int>();

    return true;
}
