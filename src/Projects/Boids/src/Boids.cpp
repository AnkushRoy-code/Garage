#include "Projects/Boids/src/Boids.h"

#include "Core/Context.h"
#include "Utils/Time.h"

BoidsContainerStruct::BoidsContainerStruct()
{
    m_BoidsVec.reserve(k_NumBoids);
}

void BoidsContainerStruct::Init()
{
    m_BoidsVec.clear();
    const float w = g_Context.AppState.ProjectWindowX;
    const float h = g_Context.AppState.ProjectWindowY;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0, w);
    std::uniform_real_distribution<> disY(0, h);
    std::uniform_real_distribution<> vel(-3.0f, 3.0f);

    for (unsigned int i = 0; i < k_NumBoids; ++i)
    {
        BoidsEntity boid {};
        boid.Position.x = disX(gen);
        boid.Position.y = disY(gen);
        boid.Velocity   = glm::vec2(vel(gen), vel(gen));
        boid.Rotation   = 0.0f;
        m_BoidsVec.push_back(boid);
    }
}

void BoidsContainerStruct::Quit()
{
    m_BoidsVec.clear();
}

void BoidsContainerStruct::ChangeData(float Seperation, float Alignment, float Cohesion)
{
    Seperation = Seperation;
    Alignment  = Alignment;
    Cohesion   = Cohesion;
}

void BoidsContainerStruct::Update()
{
    const float w = g_Context.AppState.ProjectWindowX;
    const float h = g_Context.AppState.ProjectWindowY;

    // for now I am following this tutorial:
    // https://vanhunteradams.com/Pico/Animal_Movement/Boids-algorithm.html
    for (int i = 0; i < k_NumBoids; i++)
    {
        float close_dx {}, close_dy {};
        float avg_pos_x {}, avg_pos_y {};
        float avg_vel_x {}, avg_vel_y {};
        unsigned int neighbouring_boids {};
        for (int j = 0; j < k_NumBoids; j++)
        {
            if (i == j) { continue; }
            const float dx              = m_BoidsVec[i].Position.x - m_BoidsVec[j].Position.x;
            const float dy              = m_BoidsVec[i].Position.y - m_BoidsVec[j].Position.y;
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
                avg_vel_x += m_BoidsVec[j].Velocity.x;
                avg_vel_y += m_BoidsVec[j].Velocity.y;
                avg_pos_x += m_BoidsVec[j].Position.x;
                avg_pos_y += m_BoidsVec[j].Position.y;
                neighbouring_boids++;
                continue;
            }
        }

        auto BoidVel = &m_BoidsVec[i].Velocity;
        auto BoidPos = &m_BoidsVec[i].Position;

        BoidVel->x += close_dx * Seperation;
        BoidVel->y += close_dy * Seperation;

        if (neighbouring_boids > 0)
        {
            avg_vel_x = avg_vel_x / neighbouring_boids;
            avg_vel_y = avg_vel_y / neighbouring_boids;
            avg_pos_x = avg_pos_x / neighbouring_boids;
            avg_pos_y = avg_pos_y / neighbouring_boids;
            BoidVel->x += (avg_vel_x - BoidVel->x) * Alignment;
            BoidVel->y += (avg_vel_y - BoidVel->y) * Alignment;
            BoidVel->x += (avg_pos_x - BoidPos->x) * Cohesion;
            BoidVel->y += (avg_pos_y - BoidPos->y) * Cohesion;
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

        BoidPos->x += BoidVel->x * Utils::Time::DeltaTime() / 20.0f;
        BoidPos->y += BoidVel->y * Utils::Time::DeltaTime() / 20.0f;

        // update rotation
        auto targetRotation = glm::atan(BoidVel->y, BoidVel->x);

        auto diff =
            glm::mod(targetRotation - m_BoidsVec[i].Rotation + SDL_PI_F, SDL_PI_F * 2) - SDL_PI_F;

        m_BoidsVec[i].Rotation += diff * 0.5f;
    }
}

const std::vector<BoidsEntity> &BoidsContainerStruct::GetBoids() const
{
    return m_BoidsVec;
}
