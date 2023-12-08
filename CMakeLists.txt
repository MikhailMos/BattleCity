cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# переменная имя проекта
set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME} CXX)

# компилировать под стандарт C++17
set(CMAKE_CXX_STANDARD 17)

# переменные файлов
set(RENDERER 
	src/renderer/ShaderProgram.h
	src/renderer/ShaderProgram.cpp
)

# добавление файлов в проект
add_executable(${PROJECT_NAME} 
	src/main.cpp
	${RENDERER}
)

# опции необходимые для GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# подключаем библиотеку GLFW
add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# подключаем библиотеку Glad
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)