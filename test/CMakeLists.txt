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
