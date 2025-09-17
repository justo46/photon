cmake_minimum_required(VERSION 3.23)      # 3.8 is quite old; 3.23+ is safer
project(photon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/Zc:__cplusplus /permissive- /W4)
endif()

# 1) Find deps FIRST (uses vcpkg toolchain you pass at configure)
find_package(Vulkan REQUIRED)
find_package(SDL3 CONFIG REQUIRED)

# 2) Build Dear ImGui and give it access to SDL3/Vulkan headers
add_library(imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_demo.cpp # For testing
  external/imgui/backends/imgui_impl_sdl3.cpp
  external/imgui/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
)
# Make sure the backends see SDL3/SDL.h and vulkan/vulkan.h
target_link_libraries(imgui PRIVATE SDL3::SDL3 Vulkan::Vulkan)

# 3) Your app
add_subdirectory(src/raytracer)
add_subdirectory(src/app)

add_executable(photon src/main.cpp)
target_link_libraries(photon PRIVATE app raytracer)
