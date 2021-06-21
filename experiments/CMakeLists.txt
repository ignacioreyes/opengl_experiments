#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

cmake_minimum_required(VERSION 3.10)

# Project name
project(OpenGL_experiments)

set(OpenGL_GL_PREFERENCE
  GLVND
)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

find_package(glfw3 REQUIRED)

include_directories(
  src/vendor
  ${OPENGL_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  )

link_libraries(${GLEW_LIBRARIES})

set(MY_HEADER_FILES
  src/IndexBuffer.h
  src/Shader.h
  src/VertexArray.h
  src/VertexBufferLayout.h
  src/Renderer.h
  src/Texture.h
  src/VertexBuffer.h

  src/tests/Test3D.h
  src/tests/TestCubeLight.h
  src/tests/Test.h
  src/tests/TestTexture.h
  src/tests/TestClearColor.h
  src/tests/TestFloatingHead.h
  src/tests/TestJelly.h
  src/vendor/stb_image/stb_image.h
  src/vendor/imgui/imgui.h
)

set(MY_SOURCE_FILES
  src/application.cpp
  src/Renderer.cpp
  src/Texture.cpp
  src/VertexBuffer.cpp
  src/IndexBuffer.cpp
  src/Shader.cpp
  src/VertexArray.cpp
  
  src/tests/Test3D.cpp
  src/tests/TestCubeLight.cpp
  src/tests/Test.cpp
  src/tests/TestTexture.cpp
  src/tests/TestClearColor.cpp
  src/tests/TestFloatingHead.cpp
  src/tests/TestJelly.cpp
  
  src/vendor/stb_image/stb_image.cpp

  src/vendor/imgui/imgui.cpp
  src/vendor/imgui/imgui_impl_opengl3.cpp
  src/vendor/imgui/imgui_demo.cpp
  src/vendor/imgui/imgui_tables.cpp
  src/vendor/imgui/imgui_draw.cpp
  src/vendor/imgui/imgui_widgets.cpp
  src/vendor/imgui/imgui_impl_glfw.cpp
)

add_executable(application
  ${MY_HEADER_FILES}
  ${MY_SOURCE_FILES}
)

add_dependencies(application OpenGL::OpenGL)
target_link_libraries(application OpenGL::OpenGL)
target_link_libraries(application glfw)

list(APPEND CMAKE_CXX_FLAGS "-O3 -ffast-math -march=native -mtune=native")


