# This is the executable into which you are
# supposed to include tests of your math
# library functions.

find_package(GTest CONFIG REQUIRED)

add_executable(PA199_project_tests
    "gem/gem_vector_test.cpp" # This includes the tutorial file. Can be removed.
    "gem/gem_matrix_test.cpp" 
    "gem/gem_quaternion_test.cpp"
    "gem/gem_axis_angles_test.cpp"
 "gel/gel_game_entity_test.cpp" "gel/gel_game_component_test.cpp")

# Search and ling with 3rd party libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

target_link_libraries(PA199_project_tests PUBLIC
  GTest::gtest GTest::gtest_main lodepng glad::glad glfw

  # Here add libraries you want to write tests for.
  gem # This includes the tutorial library. Can be removed.
  gel # This includes the tutorial library. Can be removed.
)

# Exposes the test (e.g., for Visual Studio).
add_test(NAME PA199_project_tests COMMAND PA199_project_tests)

# Installing the tests. Do not modify!
install(TARGETS PA199_project_tests DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gtest.dll OPTIONAL DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest.dll OPTIONAL DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gtest_main.dll OPTIONAL DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest_main.dll OPTIONAL DESTINATION .)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gtest_main.dll OPTIONAL DESTINATION .)
