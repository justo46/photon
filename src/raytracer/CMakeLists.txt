add_library(raytracer STATIC   
  vec3.h
  color.h
  raytracer.cpp
)

target_include_directories(raytracer PUBLIC ${CMAKE_SOURCE_DIR}/src)

if (MSVC)
  target_compile_options(raytracer PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()
