cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Waxedit)
set(EXE_NAME waxedit)

project(${PROJECT_NAME})

set(SOURCE_FILE_PATHS
    src/main.cpp
    src/App.cpp
    src/Window.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/IndexBuffer.cpp
    src/Renderer/VertexBuffer.cpp
    src/Renderer/OpenGL/OpenGLIndexBuffer.cpp
    src/Renderer/OpenGL/OpenGLVertexBuffer.cpp
)

add_executable(${EXE_NAME} ${SOURCE_FILE_PATHS})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
target_link_libraries(${EXE_NAME} glfw)

# FreeType
add_subdirectory(freetype)
target_link_libraries(${EXE_NAME} freetype)

# Gald
add_subdirectory(glad)
target_link_libraries(${EXE_NAME} glad)
