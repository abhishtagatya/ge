# This is a tutorial file. Feel free to remove it.

add_library(gel
	"game_entity.hpp"
	"game_entity.cpp"
	"game_component.hpp" 
	"game_component.cpp"
	"game_scene.hpp"
	"game_scene.cpp" 
	"test_component.hpp"
	"test_component.cpp" 
	"camera/camera_component.hpp"
	"camera/camera_component.cpp"
	"renderer/mesh_renderer_component.hpp"
	"renderer/mesh_renderer_component.cpp"
	"renderer/sphere_renderer_component.hpp"
	"renderer/sphere_renderer_component.cpp"
	"renderer/cube_renderer_component.hpp"
	"renderer/cube_renderer_component.cpp"
	"renderer/plane_renderer_component.hpp"
	"renderer/plane_renderer_component.cpp"
	"renderer/circle_renderer_component.hpp" 
	"renderer/cylinder_renderer_component.cpp"
	"light/light_component.hpp"
	"light/light_component.cpp"
	"gel.hpp"
  "light/directional_light_component.cpp")

target_include_directories(gel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# (optional) Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
target_link_libraries(gel PUBLIC lodepng glad::glad glfw gem)
