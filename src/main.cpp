#include <iostream>
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <random>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include "camera.h"
#include "shaders.h"
#include "model.h"

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void processInput(GLFWwindow*);
float getRandomFloat(float, float);

std::string path_to_vertex_glsl = std::string(TOSTRING(SHADER_PATH)) + "vertex.glsl";
std::string path_to_fragment_glsl = std::string(TOSTRING(SHADER_PATH)) + "fragment.glsl";
std::string path_to_grass_model = std::string(TOSTRING(MODEL_PATH)) + "grass_blade/grass_blade.obj";
std::string path_to_grass_vertex_glsl = std::string(TOSTRING(SHADER_PATH)) + "grass_vertex.glsl";
std::string path_to_grass_fragment_glsl = std::string(TOSTRING(SHADER_PATH)) + "grass_fragment.glsl";

unsigned int xWindowSize = 1080;
unsigned int yWindowSize = 720;

const int grassCount = 150;

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
         1.0f,  0.0f,  1.0f,
        -1.0f,  0.0f,  1.0f,
        -1.0f,  0.0f, -1.0f,
         1.0f,  0.0f, -1.0f
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

    Model grass(path_to_grass_model.c_str());
    Shader grassShader(path_to_grass_vertex_glsl.c_str(), path_to_grass_fragment_glsl.c_str());

    vector<glm::vec3> grassPosition;
    vector<float> grassRotation;

    for (int i = 0; i < grassCount; i++)
    {
        glm::vec3 position;
        position.x = getRandomFloat(-1.0f, 1.0f);
        position.y = 0.0f;
        position.z = getRandomFloat(-1.0f, 1.0f);

        grassPosition.push_back(position);
    }

    for (int i = 0; i < grassCount; i++)
    {
        float radians = getRandomFloat(0.0f, 180.0f);

        grassRotation.push_back(radians);
    }

    glm::vec3 cameraPosition = glm::vec3(0.0f, 5.0f, 5.0f);
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

        shaderProgram.use();
        glBindVertexArray(VAO);

        glm::mat4 view = camera.getViewMat();
        glm::mat4 model = glm::mat4(1.0f);

        shaderProgram.setMat4("projection", projection);
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        grassShader.use();

        float getTime = glfwGetTime();

        for (int i = 0; i < grassCount; i++)
        {
            //grassShader.setFloat("uTime", getTime);
            grassShader.setMat4("projection", projection);
            grassShader.setMat4("view", view);

            model = glm::mat4(1.0f);
            model = glm::translate(model, grassPosition[i]);
            model = glm::rotate(model, glm::radians(grassRotation[i]), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
            grassShader.setMat4("model", model);

            grass.Draw(grassShader);
        }

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

float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
