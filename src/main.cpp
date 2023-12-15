﻿#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include <iostream>
#include <string>
#include <chrono>

#include "game/Game.h"
#include "resources/ResourceManager.h"

glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);

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

    g_game.SetKey(key, action);
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

    glClearColor(0, 0, 0, 1);

    {
        // создание шейдеров
        ResourceManager::SetExecutablePath(argv[0]);
        g_game.Init();

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game.Update(duration);
            
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            g_game.Render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }

        ResourceManager::UnloadAllResources();
    }

    glfwTerminate();
    
    return 0;
}