# This is a tutorial file. Feel free to remove it.

add_library(mathlib
    "unit.hpp"
    "vector.hpp"
    "vector.cpp"
    "matrix.hpp"
    "matrix.cpp"
    "quaternion.cpp" 
    "axis_angle.cpp" 
    "interpolation.cpp")

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(mathlib PUBLIC lodepng glad::glad glfw)
