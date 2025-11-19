cmake_minimum_required(VERSION 3.10)

add_executable(test_button test_button.c)
target_link_libraries(test_button PRIVATE unity::framework gui)
add_test(NAME test_button COMMAND test_button)

add_executable(test_widget test_widget.c)
target_link_libraries(test_widget PRIVATE unity::framework gui)
add_test(NAME test_widget COMMAND test_widget)

add_executable(test_label test_label.c)
target_link_libraries(test_label PRIVATE unity::framework gui)
add_test(NAME test_label COMMAND test_label)

add_executable(test_container test_container.c)
target_link_libraries(test_container PRIVATE unity::framework gui)
add_test(NAME test_container COMMAND test_container)

add_executable(test_game game/test_game.c)
target_link_libraries(test_game PRIVATE unity::framework game gui)
add_test(NAME test_game COMMAND test_game)

add_executable(test_game_page game/test_game_page.c)
target_link_libraries(test_game_page PRIVATE unity::framework game gui)
add_test(NAME test_game_page COMMAND test_game_page)

add_executable(test_menu_page game/test_menu_page.c)
target_link_libraries(test_menu_page PRIVATE unity::framework game gui)
add_test(NAME test_menu_page COMMAND test_menu_page)
