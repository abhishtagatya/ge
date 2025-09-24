# This is a tutorial file. Feel free to remove it.

add_library(mathlib
    "vector.hpp"
    "vector.cpp"
    )

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(mathlib PUBLIC lodepng glad::glad glfw)
