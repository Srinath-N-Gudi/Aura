cmake_minimum_required(VERSION 3.20)
project(MinecraftClone LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD - use the existing glad.c from AuraEngine
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/../AuraEngine/src/Vendors/glad.c")

# Create executable
add_executable(MinecraftClone
    Main.cpp
    Chunk.cpp
    World.cpp
    ${GLAD_SOURCE}
)

target_include_directories(MinecraftClone PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../AuraEngine/src/Vendors
    ${CMAKE_SOURCE_DIR}/../Vendors/include
)

target_link_libraries(MinecraftClone PRIVATE glfw opengl32)

# Windows specific
if(WIN32)
    target_link_libraries(MinecraftClone PRIVATE opengl32 gdi32 user32 kernel32)
endif()

# Copy shaders to build directory
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/*.glsl")
foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    configure_file(${SHADER} "${CMAKE_BINARY_DIR}/${SHADER_NAME}" COPYONLY)
endforeach()
