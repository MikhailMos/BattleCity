cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# переменная имя проекта
set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME} CXX)

# компилировать под стандарт C++17
set(CMAKE_CXX_STANDARD 17)

# переменные для файлов
set(RENDERER 
	src/renderer/ShaderProgram.h
	src/renderer/ShaderProgram.cpp
)
set(RESOURCE_MANAGER
	src/resources/ResourceManager.h
	src/resources/ResourceManager.cpp
)

set(LOAD_IMAGE
	src/resources/stb_image.h
)

set(TEXTURE_2D
	src/renderer/Texture2D.h
	src/renderer/Texture2D.cpp
)

set(SPRITE
	src/renderer/Sprite.h
	src/renderer/Sprite.cpp
	src/renderer/AnimatedSprite.h
	src/renderer/AnimatedSprite.cpp
)

# добавление файлов в проект
add_executable(${PROJECT_NAME} 
	src/main.cpp
	${RENDERER}
	${TEXTURE_2D}
	${SPRITE}
	${RESOURCE_MANAGER}
	${LOAD_IMAGE}
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

# включаем возможность инклюдировать функции glm в нашем проекте
include_directories(external/glm)


# Чтоб исполняемый файл в месте с ресурсами лежал отедльно
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

# скопировать файлы в папку bin
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
