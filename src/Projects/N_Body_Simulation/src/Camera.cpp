#include "Projects/N_Body_Simulation/src/Camera.h"

#include "Core/Context.h"
#include "Core/EventHandler.h"
#include "Utils/Time.h"

Camera::Camera(glm::vec3 position, glm::quat orientation) :
    m_Position(position), Orientation(orientation), MovementSpeed(2.5f), MouseSensitivity(0.3f),
    Zoom(45.0f)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    glm::mat4 rotation    = glm::mat4_cast(glm::conjugate(Orientation));
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_Position);
    return rotation * translation;
}

void Camera::ProcessEvents()
{
    if ((g_Context.AppState.projectWindowFocused && g_Context.AppState.projectWindowHovered)
        || lockMouse)
    {
        ProcessKeyboard();
        if (g_Context.EventHandler.GetEventHeld(Core::MOUSE_MOVE))
        {
            ProcessMouseMovement(g_Context.AppState.mouseRel.x, g_Context.AppState.mouseRel.y);
        }
    }
}

void Camera::ProcessKeyboard()
{
    float dt       = Utils::Time::DeltaTime() / 10.0f;
    float velocity = MovementSpeed * dt;

    glm::vec3 forward = Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right   = Orientation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up      = Orientation * glm::vec3(0.0f, 1.0f, 0.0f);

    if (g_Context.EventHandler.GetEventHeld(Core::MOUSE_ROLL))
    {
        if (g_Context.AppState.VerticalScroll > 0) { m_Position += forward * velocity * 10.0f; }
        else if (g_Context.AppState.VerticalScroll < 0) { m_Position -= forward * velocity * 10.0f; }
    }
    if (g_Context.EventHandler.GetEventHeld(Core::A)) { m_Position -= right * velocity; }
    if (g_Context.EventHandler.GetEventHeld(Core::D)) { m_Position += right * velocity; }

    if (g_Context.EventHandler.GetEventHeld(Core::LEFT))
    {
        float angle   = MouseSensitivity * dt;
        glm::quat rot = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        Orientation   = glm::normalize(rot * Orientation);
    }

    if (g_Context.EventHandler.GetEventHeld(Core::RIGHT))
    {
        float angle   = MouseSensitivity * dt;
        glm::quat rot = glm::angleAxis(glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        Orientation   = glm::normalize(rot * Orientation);
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    glm::quat qYaw = glm::angleAxis(glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 right  = Orientation * glm::vec3(1.0f, 0.0f, 0.0f);
    glm::quat qPitch = glm::angleAxis(glm::radians(-yoffset), right);

    Orientation = glm::normalize(qYaw * qPitch * Orientation);

    ConstrainPitch();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if (Zoom < 1.0f) Zoom = 1.0f;
    if (Zoom > 45.0f) Zoom = 45.0f;
}

void Camera::ConstrainPitch()
{
    glm::vec3 forward = Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
    float pitch       = glm::degrees(asin(forward.y));
    if (pitch > 89.0f)
    {
        float delta = pitch - 89.0f;
        glm::quat adjust =
            glm::angleAxis(glm::radians(-delta), Orientation * glm::vec3(1.0f, 0.0f, 0.0f));
        Orientation = glm::normalize(adjust * Orientation);
    }
    else if (pitch < -89.0f)
    {
        float delta = -89.0f - pitch;
        glm::quat adjust =
            glm::angleAxis(glm::radians(delta), Orientation * glm::vec3(1.0f, 0.0f, 0.0f));
        Orientation = glm::normalize(adjust * Orientation);
    }
}
