cmake_minimum_required(VERSION 3.23)      # 3.8 is quite old; 3.23+ is safer
project(photon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/Zc:__cplusplus /permissive- /W4)
endif()

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED) 
find_package(glfw3 CONFIG REQUIRED)  


add_library(imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  # external/imgui/imgui_demo.cpp # For testing
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE OpenGL::GL glfw glad::glad)

add_subdirectory(src/raytracer)
add_subdirectory(src/app)

add_executable(photon src/main.cpp)
target_link_libraries(photon PRIVATE app raytracer)
