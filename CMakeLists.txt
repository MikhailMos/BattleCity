cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# ���������� ��� �������
set(PROJECT_NAME BattleCity)

project(${PROJECT_NAME} CXX)

# ������������� ��� �������� C++17
set(CMAKE_CXX_STANDARD 17)

# ���������� ��� ������
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
	src/renderer/AnimatedSprite.h
	src/renderer/AnimatedSprite.cpp
)

set(GAME
	src/game/Game.h
	src/game/Game.cpp
	src/game/Level.h
	src/game/Level.cpp
)

set(GAME_OBJECTS
	src/game/GameObjects/Tank.h
	src/game/GameObjects/Tank.cpp
	src/game/GameObjects/iGameObject.h
	src/game/GameObjects/iGameObject.cpp
	src/game/GameObjects/BrickWall.h
	src/game/GameObjects/BrickWall.cpp
)

# ���������� ������ � ������
add_executable(${PROJECT_NAME} 
	src/main.cpp
	${RENDERER}
	${TEXTURE_2D}
	${SPRITE}
	${RESOURCE_MANAGER}
	${LOAD_IMAGE}
	${GAME}
	${GAME_OBJECTS}
)

# ����� ����������� ��� GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# ���������� ���������� GLFW
add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# ���������� ���������� Glad
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

# �������� ����������� ������������� ������� glm � ����� �������
include_directories(external/glm)

# ���������� ���������� RapidJSON
include_directories(external/rapidjson/include)

# ���� ����������� ���� � ����� � ��������� ����� ��������
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

# ����������� ����� � ����� bin
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
