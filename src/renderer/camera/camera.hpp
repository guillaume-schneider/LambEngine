#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <input.hpp>

/**
 * @brief Represents a camera in a 3D scene.
 */
class Camera
{
public:
    /**
     * @brief Constructs a Camera object.
     *
     * @param position The initial position of the camera.
     * @param direction The initial direction of the camera.
     * @param up The up vector of the camera.
     * @param mouseSensitivity The sensitivity of the mouse movement.
     * @param cameraSpeed The speed of the camera movement.
     */
    Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float mouseSensitivity = 0.1f, float cameraSpeed = 2.5f)
        : m_position(position), m_direction(direction), m_up(up), m_mouseSensitivity(mouseSensitivity),
          m_cameraSpeed(cameraSpeed) {};

    /**
     * @brief Computes the camera movements based on cursor position changes.
     *
     * @param x The x-coordinate of the cursor position.
     * @param y The y-coordinate of the cursor position.
     */
    void computeCursorCameraMovements(int x, int y);

    /**
     * @brief Computes the camera actions based on a vector of actions.
     *
     * @param actions The vector of actions to be computed.
     */
    void computeActions(const std::vector<Action>& actions);

    /**
     * @brief Gets the view matrix of the camera.
     *
     * @return The view matrix of the camera.
     */
    glm::mat4 getViewMatrix() const { return glm::lookAt(m_position, m_position + m_direction, m_up); };

    /**
     * @brief Gets the position of the camera.
     *
     * @return The position of the camera.
     */
    glm::vec3 getPosition() const { return m_position; }

    /**
     * @brief Gets the direction of the camera.
     *
     * @return The direction of the camera.
     */
    glm::vec3 getDirection() const { return m_direction; }

private:
    // Camera position
    glm::vec3 m_position;
    // Camera direction
    glm::vec3 m_direction;
    // Up vector
    glm::vec3 m_up;
    // Yaw angle
    float yaw = -90.0f;
    // Pitch angle
    float pitch = 0.0f;
    // Sensitivity for mouse movement
    float m_mouseSensitivity;
    // Speed of the camera movement
    float m_cameraSpeed;

    /**
     * @brief Computes a single action for the camera.
     *
     * @param action The action to be computed.
     */
    void computeAction(Action action);
};

#endif
