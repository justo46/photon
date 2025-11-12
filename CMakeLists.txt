cmake_minimum_required(VERSION 3.23)
project(photon LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/Zc:__cplusplus /permissive- /W4)
endif()

# --- OpenGL (system) ---
find_package(OpenGL REQUIRED)

# --- GLFW (vendored) ---
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# --- GLAD (vendored) ---
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- ImGui (vendored) ---
add_library(imgui STATIC
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  external/imgui
  external/imgui/backends
)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glad OpenGL::GL glfw)

# --- Your own libs/apps ---
add_subdirectory(src/raytracer)
add_subdirectory(src/app)

add_executable(photon src/main.cpp)
target_link_libraries(photon PRIVATE app raytracer)
