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

add_executable(test_container_layout test_container_layout.c)
target_link_libraries(test_container_layout PRIVATE unity::framework gui)
add_test(NAME test_container_layout COMMAND test_container_layout)

add_executable(test_container_dirty test_container_dirty.c)
target_link_libraries(test_container_dirty PRIVATE unity::framework gui)
add_test(NAME test_container_dirty COMMAND test_container_dirty)

add_executable(test_grid_layout test_grid_layout.c)
target_link_libraries(test_grid_layout PRIVATE unity::framework gui)
add_test(NAME test_grid_layout COMMAND test_grid_layout)

add_executable(test_alignment_justify test_alignment_justify.c)
target_link_libraries(test_alignment_justify PRIVATE unity::framework gui)
add_test(NAME test_alignment_justify COMMAND test_alignment_justify)

add_executable(test_container_edge_cases test_container_edge_cases.c)
target_link_libraries(test_container_edge_cases PRIVATE unity::framework gui)
add_test(NAME test_container_edge_cases COMMAND test_container_edge_cases)

add_executable(test_button_dirty test_button_dirty.c)
target_link_libraries(test_button_dirty PRIVATE unity::framework gui)
add_test(NAME test_button_dirty COMMAND test_button_dirty)

add_executable(test_canvas test_canvas.c)
target_link_libraries(test_canvas PRIVATE unity::framework gui)
add_test(NAME test_canvas COMMAND test_canvas)
