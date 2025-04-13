/// This is a simple camera thingy for one proj only
#ifndef INCLUDE_SRC_CAMERA_H_
#define INCLUDE_SRC_CAMERA_H_

#include "Core/Common/pch.h"

class Camera
{
  public:
    explicit Camera(glm::vec3 position    = glm::vec3(0.0f, 0.0f, -3.0f),
                    glm::quat orientation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f)));

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    void ProcessEvents();

    bool lockMouse = false;

  private:
    glm::vec3 m_Position;
    glm::quat Orientation;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

  private:
    void ProcessKeyboard();
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

    void ConstrainPitch();
};

#endif  // INCLUDE_SRC_CAMERA_H_
