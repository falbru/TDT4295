cmake_minimum_required(VERSION 3.10)

add_executable(test_button test_button.c)
target_link_libraries(test_button PRIVATE unity::framework gui)
add_test(NAME test_button COMMAND test_button)
