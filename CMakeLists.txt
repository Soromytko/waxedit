cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Waxedit)
set(EXE_NAME waxedit)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILE_PATHS
    src/main.cpp
    src/App.cpp
    src/Window.cpp
    src/FontRaster.cpp
    src/TextRenderer.cpp

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

# Rendell
add_subdirectory(rendell)
target_link_libraries(${EXE_NAME} rendell)

# Glm
add_subdirectory(glm)
target_link_libraries(${EXE_NAME} glm)
