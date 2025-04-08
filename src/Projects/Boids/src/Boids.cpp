#include "Boids.h"
#include "Core/Context.h"
#include "Utils/Time.h"

BoidsContainer::BoidsContainer()
{
    BoidsVec.reserve(NUM_BOIDS);
}

void BoidsContainer::init()
{
    BoidsVec.clear();
    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0, w);
    std::uniform_real_distribution<> disY(0, h);
    std::uniform_real_distribution<> vel(-3.0f, 3.0f);

    for (unsigned int i = 0; i < NUM_BOIDS; ++i)
    {
        BoidsEntity boid {};
        boid.Position.x = disX(gen);
        boid.Position.y = disY(gen);
        boid.Velocity   = glm::vec2(vel(gen), vel(gen));
        boid.Rotation   = 0.0f;
        BoidsVec.push_back(boid);
    }
}

void BoidsContainer::quit() {
    BoidsVec.clear();
}

void BoidsContainer::changeData(float Seperation, float Alignment, float Cohesion)
{
    seperation = Seperation;
    alignment  = Alignment;
    cohesion   = Cohesion;
}

void BoidsContainer::update()
{
    const float w = gContext.appState.ProjectWindowX;
    const float h = gContext.appState.ProjectWindowY;

    // for now I am following this tutorial:
    // https://vanhunteradams.com/Pico/Animal_Movement/Boids-algorithm.html
    for (int i = 0; i < NUM_BOIDS; i++)
    {
        float close_dx {}, close_dy {};
        float avg_pos_x {}, avg_pos_y {};
        float avg_vel_x {}, avg_vel_y {};
        unsigned int neighbouring_boids {};
        for (int j = 0; j < NUM_BOIDS; j++)
        {
            if (i == j) { continue; }
            const float dx              = BoidsVec[i].Position.x - BoidsVec[j].Position.x;
            const float dy              = BoidsVec[i].Position.y - BoidsVec[j].Position.y;
            const float dist            = dx * dx + dy * dy;
            const int nearRadiusSquared = 256;   // 16
            const int farRadiusSquared  = 2500;  // 50

            if (dist > farRadiusSquared)  // too far
            {
                continue;
            }

            if (dist < nearRadiusSquared)
            {
                close_dx += dx;
                close_dy += dy;
                continue;
            }

            if (dist < farRadiusSquared)
            {
                avg_vel_x += BoidsVec[j].Velocity.x;
                avg_vel_y += BoidsVec[j].Velocity.y;
                avg_pos_x += BoidsVec[j].Position.x;
                avg_pos_y += BoidsVec[j].Position.y;
                neighbouring_boids++;
                continue;
            }
        }

        auto BoidVel = &BoidsVec[i].Velocity;
        auto BoidPos = &BoidsVec[i].Position;

        BoidVel->x += close_dx * seperation;
        BoidVel->y += close_dy * seperation;

        if (neighbouring_boids > 0)
        {
            avg_vel_x = avg_vel_x / neighbouring_boids;
            avg_vel_y = avg_vel_y / neighbouring_boids;
            avg_pos_x = avg_pos_x / neighbouring_boids;
            avg_pos_y = avg_pos_y / neighbouring_boids;
            BoidVel->x += (avg_vel_x - BoidVel->x) * alignment;
            BoidVel->y += (avg_vel_y - BoidVel->y) * alignment;
            BoidVel->x += (avg_pos_x - BoidPos->x) * cohesion;
            BoidVel->y += (avg_pos_y - BoidPos->y) * cohesion;
        }

        const int margin       = 100;
        const float turnfactor = 0.2f;

        if (BoidPos->x < margin) { BoidVel->x += turnfactor; }
        else if (BoidPos->x > w - margin) { BoidVel->x -= turnfactor; }
        if (BoidPos->y < margin) { BoidVel->y += turnfactor; }
        else if (BoidPos->y > h - margin) { BoidVel->y -= turnfactor; }

        float speed          = glm::sqrt(BoidVel->x * BoidVel->x + BoidVel->y * BoidVel->y);
        const float minSpeed = 3.0f;
        const float maxSpeed = 6.0f;

        if (speed < minSpeed)
        {
            if (speed == 0.0f)
            {
                BoidVel->x = minSpeed;
                BoidVel->y = 0.0f;
            }
            else
            {
                BoidVel->x = (BoidVel->x / speed) * minSpeed;
                BoidVel->y = (BoidVel->y / speed) * minSpeed;
            }
        }
        else if (speed > maxSpeed)
        {
            BoidVel->x = (BoidVel->x / speed) * maxSpeed;
            BoidVel->y = (BoidVel->y / speed) * maxSpeed;
        }

        BoidPos->x += BoidVel->x * Utils::Time::deltaTime() / 20.0f;
        BoidPos->y += BoidVel->y * Utils::Time::deltaTime() / 20.0f;

        // update rotation
        auto targetRotation = glm::atan(BoidVel->y, BoidVel->x);

        auto diff =
            glm::mod(targetRotation - BoidsVec[i].Rotation + SDL_PI_F, SDL_PI_F * 2) - SDL_PI_F;

        BoidsVec[i].Rotation += diff * 0.5f;
    }
}

const std::vector<BoidsEntity> &BoidsContainer::getBoids() const
{
    return BoidsVec;
}
