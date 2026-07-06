#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float Speed = 10.0f;
    float Sensitivity = 0.1f;
    float FOV = 70.0f;
    float NearPlane = 0.1f;
    float FarPlane = 500.0f;

    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera(glm::vec3 position = glm::vec3(0, 32, 0))
        : Position(position) {
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix(float aspect) const {
        return glm::perspective(glm::radians(FOV), aspect, NearPlane, FarPlane);
    }

    void ProcessKeyboard(GLFWwindow* window, float deltaTime) {
        float velocity = Speed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            velocity *= 3.0f;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position -= Front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position -= Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += Right * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Position += WorldUp * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            Position -= WorldUp * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        updateCameraVectors();
    }

    glm::vec3 GetForward() const { return Front; }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
