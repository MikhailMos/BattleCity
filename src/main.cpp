﻿#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>
#include <chrono>

#include "renderer/Texture2D.h"
#include "renderer/ShaderProgram.h"
#include "renderer/Sprite.h"
#include "renderer/AnimatedSprite.h"
#include "resources/ResourceManager.h"


GLfloat point[] = {
     0.0f,  50.f, 0.0f,
     50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoords[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit()) 
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow*  pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle City", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // регистрация коллбеков
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL()) 
	{
		std::cout << "Can't load GLAD!" << std::endl;
		return -1;
	}

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << " \nVendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 1, 0, 1);

    {
        // создание шейдеров
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.LoadShaders("DefaultShader", "resources/shaders/vertext.txt", "resources/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't creatre shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.LoadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't creatre sprite shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.LoadTexture("DefaultTexture", "resources/textures/map_16x16.png");

        std::vector<std::string> subTexturesNames = {
            /*"block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "topLeftBlock", "topRigthBlock", 
            "bottomLeftBlock", "bottomRightBlock", "beton",*/
            "eagle",                "deadEagle",             "respawn1",             "respawn2",              "respawn3",   "respawn4",   "shield1",    "shield2",
            "explosionTopLeftBig1", "explosionTopRightBig1", "explosionTopLeftBig2", "explosionTopRightBig2", "explosion1", "explosion2", "explosion3", "nothing"
        };
        auto pTextureAtlas = resourceManager.LoadTextureAtlas("DefaultTextureAtlas", "resources/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

        auto pSprite = resourceManager.LoadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "shield1");
        pSprite->SetPosition(glm::vec2(300, 100));
        
        auto pAnimatedSprite = resourceManager.LoadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "eagle");
        pAnimatedSprite->SetPosition(glm::vec2(300, 300));

        std::vector<std::pair<std::string, uint64_t>> respawnState;
        respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn1", 1000000000));
        respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn2", 1000000000));
        respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn3", 1000000000));
        respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 1000000000));

        std::vector<std::pair<std::string, uint64_t>> eagleState;
        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
        eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));
        
        pAnimatedSprite->InsertState("RespawnState", std::move(respawnState));
        pAnimatedSprite->InsertState("EagleState", std::move(eagleState));
        
        pAnimatedSprite->SetState("RespawnState");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->Use();
        pDefaultShaderProgram->SetInt("tex", 0);

        glm::mat4 modelMatrix1 = glm::mat4(1.f);
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(50.f, 50.f, 0.f));

        glm::mat4 modelMatrix2 = glm::mat4(1.f);
        modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(590.f, 50.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->Use();
        pSpriteShaderProgram->SetInt("tex", 0);
        pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            pAnimatedSprite->Update(duration);
            
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->Use();
            glBindVertexArray(vao);
            tex->Bind();

            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->SetMatrix4("modelMat", modelMatrix2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->Render();

            pAnimatedSprite->Render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    
    return 0;
}