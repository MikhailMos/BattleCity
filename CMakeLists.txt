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
	src/renderer/VertexBuffer.h
	src/renderer/VertexBuffer.cpp
	src/renderer/IndexBuffer.h
	src/renderer/IndexBuffer.cpp
	src/renderer/VertexArray.h
	src/renderer/VertexArray.cpp
	src/renderer/VertexBufferLayout.h
	src/renderer/VertexBufferLayout.cpp
	src/renderer/Renderer.h
	src/renderer/Renderer.cpp
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
	src/renderer/SpriteAnimator.h
	src/renderer/SpriteAnimator.cpp
)

set(GAME
	src/game/Game.h
	src/game/Game.cpp
	src/game/GameStates/Level.h
	src/game/GameStates/Level.cpp
	src/game/GameStates/IGameState.h
	src/game/GameStates/StartScreen.cpp
	src/game/GameStates/StartScreen.h
)

set(GAME_OBJECTS
	src/game/GameObjects/Tank.h
	src/game/GameObjects/Tank.cpp
	src/game/GameObjects/iGameObject.h
	src/game/GameObjects/iGameObject.cpp
	src/game/GameObjects/BrickWall.h
	src/game/GameObjects/BrickWall.cpp
	src/game/GameObjects/BetonWall.h
	src/game/GameObjects/BetonWall.cpp
	src/game/GameObjects/Trees.h
	src/game/GameObjects/Trees.cpp
	src/game/GameObjects/Ice.h
	src/game/GameObjects/Ice.cpp
	src/game/GameObjects/Water.h
	src/game/GameObjects/Water.cpp
	src/game/GameObjects/Eagle.h
	src/game/GameObjects/Eagle.cpp
	src/game/GameObjects/Border.h
	src/game/GameObjects/Border.cpp
	src/game/GameObjects/Bullet.h
	src/game/GameObjects/Bullet.cpp
)

set(GAME_SYSTEM
	src/game_system/Timer.h
	src/game_system/Timer.cpp
)

set(PHYSICS
	src/physics/PhysicsEngine.h
	src/physics/PhysicsEngine.cpp
)

# добавление файлов в проект
add_executable(${PROJECT_NAME} 
	src/main.cpp
	${RENDERER}
	${TEXTURE_2D}
	${SPRITE}
	${RESOURCE_MANAGER}
	${LOAD_IMAGE}
	${GAME}
	${GAME_OBJECTS}
	${GAME_SYSTEM}
	${PHYSICS}
)

# устанавливаем наш проект стартовым по умолчанию (нужно для VS)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

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
#include_directories(external/glm)
add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

# подключаем библиотеку RapidJSON
include_directories(external/rapidjson/include)

# Чтоб исполняемый файл в месте с ресурсами лежал отедльно
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

# скопировать файлы в папку bin
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
