add_executable(photon
  photon.cpp
  photon.h
  # add more sources as needed
)

# (Optional) explicit C++ standard if you want
set_property(TARGET photon PROPERTY CXX_STANDARD 23)

# Link your app to the deps + imgui
target_link_libraries(photon PRIVATE Vulkan::Vulkan SDL3::SDL3 imgui)
