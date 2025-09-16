# Make 'app' a library (NOT an executable)
add_library(app STATIC
  app.cpp
  app.h
)

target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(app PRIVATE SDL3::SDL3 Vulkan::Vulkan imgui)

if (MSVC)
  target_compile_options(app PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()
