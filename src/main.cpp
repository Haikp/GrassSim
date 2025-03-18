#include <iostream>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "camera.h"
#include "shaders.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void processInput(GLFWwindow*);

std::string path_to_vertex_glsl = std::string(TOSTRING(SHADER_PATH)) + "vertex.glsl";
std::string path_to_fragment_glsl = std::string(TOSTRING(SHADER_PATH)) + "fragment.glsl";

unsigned int xWindowSize = 1080;
unsigned int yWindowSize = 720;

int main()
{
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(xWindowSize, yWindowSize, "Grass Sim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to init glew" << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader shaderProgram(path_to_vertex_glsl.c_str(), path_to_fragment_glsl.c_str());
    shaderProgram.use();

    float vertices[] = {
        //coordinates
         0.5f,  0.0f,  0.5f,
        -0.5f,  0.0f,  0.5f,
        -0.5f,  0.0f, -0.5f,
         0.5f,  0.0f, -0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraOrientation = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera camera(cameraPosition, cameraForward, cameraOrientation, xWindowSize, yWindowSize);

    float fov = 90.0f;
    glm::mat4 projection = glm::perspective(glm::radians(fov), ((float)xWindowSize / (float)yWindowSize), 0.1f, 100.0f);
    shaderProgram.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.0f, .0f, .0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.TakeInputs(window);

        glm::mat4 view = camera.getViewMat();
        glm::mat4 model = glm::mat4(1.0f);

        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
