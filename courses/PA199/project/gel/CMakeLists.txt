# This is a tutorial file. Feel free to remove it.

add_library(gel
	"game_entity.hpp"
	"game_entity.cpp"
 "game_component.cpp" "game_scene.cpp" "test_component.cpp")

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(gel PUBLIC lodepng glad::glad glfw gem)
