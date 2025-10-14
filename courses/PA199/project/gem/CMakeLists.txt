# This is a tutorial file. Feel free to remove it.

add_library(gem
    "vector.hpp"
    "vector.cpp"

    "matrix.hpp"
    "matrix.cpp"

    "quaternion.hpp"
    "quaternion.cpp"
    
    "axis_angle.hpp"
    "axis_angle.cpp"
    
    "interpolation.hpp"
    "interpolation.cpp"
    
    "coordinates.hpp"
    "coordinates.cpp"

    "gem.hpp"
)

target_include_directories(gem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(gem PUBLIC lodepng glad::glad glfw)
