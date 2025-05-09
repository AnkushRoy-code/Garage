#include <catch2/catch_all.hpp>
#include <chrono>
#include <thread>

#include "Utils/CapFPS.h"
#include "Utils/Time.h"
#include "Utils/Timer.h"

TEST_CASE("CapZone FPS Enforcement", "[utils][timing]")
{
    constexpr unsigned int TEST_FPS = 1000;  // 1ms frame duration
    constexpr auto MARGIN           = std::chrono::microseconds(500);

    auto start = std::chrono::high_resolution_clock::now();
    {
        Utils::CapZone cap(TEST_FPS);
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;
    auto expected = std::chrono::milliseconds(1000 / TEST_FPS);
    REQUIRE(duration >= (expected - MARGIN));
    REQUIRE(duration <= (expected + MARGIN * 2));
}

TEST_CASE("Time Utilities", "[utils][timing]")
{
    SECTION("Initialization")
    {
        Utils::Time::Init();
        REQUIRE(Utils::Time::GetTicks() <= 1);
    }

    SECTION("Delta Time Calculation")
    {
        Utils::Time::Init();

        auto delta = Utils::Time::DeltaTime();
        REQUIRE(delta == 0.0);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        Utils::Time::UpdateDeltaTime();

        delta = Utils::Time::DeltaTime();

        REQUIRE(delta >= 9.0);
        REQUIRE(delta <= 11.0);
    }

    SECTION("FPS Capping")
    {
        Utils::Time::Init();
        Utils::Time::UpdateDeltaTime();

        auto start = Utils::Time::GetTicks();
        Utils::Time::CapFPS();
        Utils::Time::UpdateDeltaTime();

        REQUIRE(Utils::Time::DeltaTime() >= 15.0);
        REQUIRE(Utils::Time::DeltaTime() <= 18.0);
    }
}

TEST_CASE("Timer Measurement", "[utils][timing]")
{
    constexpr float EXPECTED_DURATION = 50.0f;
    constexpr float TOLERANCE         = 5.0f;

    float measured = 0.0f;

    auto test = [&]()
    {
        Timer timer(measured);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(EXPECTED_DURATION)));
    };

    test();

    REQUIRE(measured >= (EXPECTED_DURATION - TOLERANCE));
    REQUIRE(measured <= (EXPECTED_DURATION + TOLERANCE));
}

TEST_CASE("Timer Microsecond Precision", "[utils][timing]")
{
    float measured = 0.0f;

    {
        Timer timer(measured);
    }

    REQUIRE(measured < 1.0f);
}
