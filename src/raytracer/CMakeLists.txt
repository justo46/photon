add_library(raytracer STATIC   
  Vec3.h
  Color.h
  Ray.h
  Hittable.h
  Sphere.h
  Image.h
  Image.cpp
  Camera.h
  Camera.cpp
  math_utils.h
  Interval.h
  Raytracer.h
  Raytracer.cpp
)

target_include_directories(raytracer PUBLIC ${CMAKE_SOURCE_DIR}/src)

if (MSVC)
  target_compile_options(raytracer PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()
