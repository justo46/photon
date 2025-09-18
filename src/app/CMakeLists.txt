# Make 'app' a library (NOT an executable)
add_library(app STATIC
	Application.cpp
	Application.h
	Rendering.cpp
	Rendering.h
)

target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(app PRIVATE SDL3::SDL3 OpenGL::GL glad::glad imgui)

if (MSVC)
	target_compile_options(app PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()
