#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

class Camera{
public:
    glm::mat4 view = glm::mat4(1.0f);

    Camera(glm::vec3 cameraPosition,
           glm::vec3 cameraForward,
           glm::vec3 cameraUp,
           unsigned int xWindowSize,
           unsigned int yWindowSize);
    void TakeInputs(GLFWwindow*);
    glm::mat4 getViewMat();
    glm::vec3 getPosition();
    glm::vec3 getFront();
    void setMouseSens(float);
    void setCameraSpeed(float);

private:
    static glm::vec3 cameraPosition;
    static glm::vec3 cameraForward;
    static glm::vec3 cameraUp;
    static bool firstMouse;
    static bool cameraActive;
    static float lastX;
    static float lastY;
    static float xoffset;
    static float yoffset;
    static float mouseSens;
    static float yaw;
    static float pitch;
    float cameraSpeed = 2.5f;
    float deltaTime = 0.0f;
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
    static void mouse_callback(GLFWwindow*, double, double);
    static void mouse_button_callback(GLFWwindow*, int, int, int);
};

#endif
