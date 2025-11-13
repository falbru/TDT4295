cmake_minimum_required(VERSION 3.10)

add_executable(test_button test_button.c)
target_link_libraries(test_button PRIVATE unity::framework gui)
add_test(NAME test_button COMMAND test_button)

add_executable(test_game test_game.c)
target_link_libraries(test_game PRIVATE unity::framework game gui)
add_test(NAME test_game COMMAND test_game)
