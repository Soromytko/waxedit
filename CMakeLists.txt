cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME Waxedit)
set(EXE_NAME waxedit)

project(${PROJECT_NAME})

add_executable(${EXE_NAME} src/main.cpp)

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
