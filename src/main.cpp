#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include <iostream>
#include <string>
#include <chrono>

#include "game/Game.h"
#include "resources/ResourceManager.h"
#include "renderer/Renderer.h"

const int LEVEL_WIDHT = 13;
const int LEVEL_HEIGHT = 14;
const int BLOCK_WIDTH = 16;
const int BLOCK_HEIGHT = 16;

glm::ivec2 g_windowSize((LEVEL_WIDHT * BLOCK_WIDTH), (LEVEL_HEIGHT * BLOCK_HEIGHT));
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
   
    RenderEngine::Renderer::SetViewport(width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

    g_game->SetKey(key, action);
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

    std::cout << "Renderer: " << RenderEngine::Renderer::GetRendererStr() << " \nVendor: " << RenderEngine::Renderer::GetVendorStr() << std::endl;
    std::cout << "OpenGL version: " << RenderEngine::Renderer::GetVersionStr() << std::endl;

    RenderEngine::Renderer::SetClearColor(0, 0, 0, 1);

    {
        // создание шейдеров
        ResourceManager::SetExecutablePath(argv[0]);
        g_game->Init();

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Poll for and process events */
            glfwPollEvents();
            
            /* Обновляем все объекты, которые есть в игровом мире */
            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;

            g_game->Update(duration);
            
            /* Render here */
            // очищаем экран
            RenderEngine::Renderer::Clear();
            // рисуем картинку
            g_game->Render();

            /* Swap front and back buffers */
            // выводим картинку на монитор
            glfwSwapBuffers(pWindow);
        }
        g_game = nullptr;
        ResourceManager::UnloadAllResources();
    }

    glfwTerminate();
    
    return 0;
}