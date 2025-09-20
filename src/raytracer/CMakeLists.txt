add_library(raytracer STATIC   
  Vec3.h
  Color.h
  Color.cpp
  Ray.h
  Hittable.h
  HittableList.h
  Sphere.h
  Camera.h
  Camera.cpp
  math_utils.h
  Interval.h
  Raytracer.h
  Raytracer.cpp
  Materials/AllMaterials.h
  Materials/Material.h
  Materials/Lambertian.h
  Materials/Metal.h
  Materials/Dielectric.h
)

target_include_directories(raytracer PUBLIC ${CMAKE_SOURCE_DIR}/src)

if (MSVC)
  target_compile_options(raytracer PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()
