cmake_minimum_required(VERSION 3.17)
project(OpenGL)

find_package(OpenGL REQUIRED)

add_executable(OpenGL Application.cpp glad.c primitives/Shader.cpp primitives/Shader.h)

target_include_directories(OpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)

if (APPLE)
    target_link_libraries(OpenGL ${OPENGL_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw.3.dylib)
endif()

if (UNIX)
    target_link_libraries(OpenGL ${OPENGL_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw.so ${CMAKE_DL_LIBS})
endif()

target_include_directories(OpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include)
target_include_directories(OpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/KHR/include)
