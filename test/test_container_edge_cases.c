#include "unity.h"
#include "widgets/container.h"
#include "widgets/label.h"
#include "widgets/widget.h"
#include <stdlib.h>

static Widget *container;

void setUp(void)
{
    container = NULL;
}

void tearDown(void)
{
    if (container)
    {
        widget_destroy(container);
        free(container);
        container = NULL;
    }
}

void test_empty_container_layout(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);

    container_update_layout(container);

    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(container));
}

void test_all_children_invisible(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    widget_set_visible(child1, false);
    widget_set_visible(child2, false);

    container_update_layout(container);
}

void test_container_with_zero_dimensions(void)
{
    container = container_create(0, 0, 0, 0, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_NOT_NULL(container);
}

void test_container_with_very_large_dimensions(void)
{
    container = container_create(0, 0, 10000, 10000, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(50, child->width);
    TEST_ASSERT_EQUAL_INT(50, child->height);
}

void test_container_with_negative_position(void)
{
    container = container_create(-100, -100, 200, 200, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(-100, child->x);
    TEST_ASSERT_EQUAL_INT(-100, child->y);
}

void test_very_large_spacing(void)
{
    container = container_create(0, 0, 10000, 100, LAYOUT_TYPE_HBOX);
    container_set_spacing(container, 5000);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(5050, child2->x);
}

void test_very_large_padding(void)
{
    container = container_create(0, 0, 10000, 10000, LAYOUT_TYPE_VBOX);
    container_set_padding(container, 1000);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(1000, child->x);
    TEST_ASSERT_EQUAL_INT(1000, child->y);
}

void test_adding_many_children(void)
{
    container = container_create(0, 0, 1000, 10000, LAYOUT_TYPE_VBOX);

    for (int i = 0; i < 100; i++)
    {
        Widget *child = label_create(0, 0, "X");
        widget_set_size(child, 50, 50);
        container_add_child(container, child);
    }

    TEST_ASSERT_EQUAL_INT(100, container_get_child_count(container));
}

void test_removing_all_children_one_by_one(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(3, container_get_child_count(container));

    container_remove_child(container, child3);
    TEST_ASSERT_EQUAL_INT(2, container_get_child_count(container));

    container_remove_child(container, child2);
    TEST_ASSERT_EQUAL_INT(1, container_get_child_count(container));

    container_remove_child(container, child1);
    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(container));
}

void test_grid_with_more_columns_than_children(void)
{
    container = container_create(0, 0, 500, 100, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 10);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child2->x);
}

void test_grid_with_very_large_column_count(void)
{
    container = container_create(0, 0, 10000, 100, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 100);

    Widget *child = label_create(0, 0, "A");
    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(0, child->y);
}

void test_rapidly_changing_layout_type(void)
{
    container = container_create(0, 0, 300, 300, LAYOUT_TYPE_NONE);

    Widget *child = label_create(10, 20, "A");
    widget_set_size(child, 50, 50);
    container_add_child(container, child);

    ContainerData *data = (ContainerData *)container->data;

    data->layout_type = LAYOUT_TYPE_HBOX;
    container_update_layout(container);

    data->layout_type = LAYOUT_TYPE_VBOX;
    container_update_layout(container);

    data->layout_type = LAYOUT_TYPE_GRID;
    container_update_layout(container);

    data->layout_type = LAYOUT_TYPE_NONE;
    container_update_layout(container);

    TEST_ASSERT_NOT_NULL(container);
}

void test_child_capacity_expansion(void)
{
    container = container_create(0, 0, 1000, 1000, LAYOUT_TYPE_VBOX);

    ContainerData *data = (ContainerData *)container->data;
    int initial_capacity = data->child_capacity;

    for (int i = 0; i < initial_capacity + 5; i++)
    {
        Widget *child = label_create(0, 0, "X");
        container_add_child(container, child);
    }

    TEST_ASSERT_GREATER_THAN(initial_capacity, data->child_capacity);
    TEST_ASSERT_EQUAL_INT(initial_capacity + 5, container_get_child_count(container));
}

void test_padding_larger_than_container(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);
    container_set_padding(container, 200);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(200, child->x);
    TEST_ASSERT_EQUAL_INT(200, child->y);
}

void test_spacing_larger_than_container(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_HBOX);
    container_set_spacing(container, 500);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(550, child2->x);
}

void test_hbox_with_no_available_width(void)
{
    container = container_create(0, 0, 50, 100, LAYOUT_TYPE_HBOX);
    container_set_padding(container, 30);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 0, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(30, child->x);
    TEST_ASSERT_EQUAL_INT(30, child->y);
}

void test_vbox_with_no_available_height(void)
{
    container = container_create(0, 0, 100, 50, LAYOUT_TYPE_VBOX);
    container_set_padding(container, 30);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 0);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(30, child->x);
    TEST_ASSERT_EQUAL_INT(30, child->y);
}

void test_deeply_nested_containers(void)
{
    container = container_create(0, 0, 400, 400, LAYOUT_TYPE_VBOX);
    Widget *level1 = container_create(0, 0, 300, 300, LAYOUT_TYPE_HBOX);
    Widget *level2 = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *level3 = container_create(0, 0, 100, 100, LAYOUT_TYPE_GRID);
    Widget *child = label_create(0, 0, "Deep");

    widget_set_size(child, 50, 50);

    container_add_child(container, level1);
    container_add_child(level1, level2);
    container_add_child(level2, level3);
    container_add_child(level3, child);

    TEST_ASSERT_NOT_NULL(container);
    TEST_ASSERT_NOT_NULL(child);
}

void test_mixed_visible_invisible_children_hbox(void)
{
    container = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");
    Widget *child4 = label_create(0, 0, "D");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);
    widget_set_size(child4, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    widget_set_visible(child2, false);
    widget_set_visible(child4, false);

    container_update_layout(container);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child3->x);
}

void test_mixed_visible_invisible_children_grid(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");
    Widget *child4 = label_create(0, 0, "D");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    widget_set_visible(child2, false);
    widget_set_visible(child3, false);

    TEST_ASSERT_EQUAL_INT(4, container_get_child_count(container));
}

void test_adding_child_to_itself(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "Child");
    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(1, container_get_child_count(container));
}

void test_container_destroy_with_many_children(void)
{
    container = container_create(0, 0, 1000, 1000, LAYOUT_TYPE_VBOX);

    for (int i = 0; i < 50; i++)
    {
        Widget *child = label_create(0, 0, "X");
        container_add_child(container, child);
    }

    widget_destroy(container);
    free(container);
    container = NULL;

    TEST_ASSERT_NULL(container);
}

void test_zero_spacing_and_padding(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_HBOX);
    container_set_spacing(container, 0);
    container_set_padding(container, 0);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(50, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child2->y);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_empty_container_layout);
    RUN_TEST(test_all_children_invisible);
    RUN_TEST(test_container_with_zero_dimensions);
    RUN_TEST(test_container_with_very_large_dimensions);
    RUN_TEST(test_container_with_negative_position);
    RUN_TEST(test_very_large_spacing);
    RUN_TEST(test_very_large_padding);
    RUN_TEST(test_adding_many_children);
    RUN_TEST(test_removing_all_children_one_by_one);
    RUN_TEST(test_grid_with_more_columns_than_children);
    RUN_TEST(test_grid_with_very_large_column_count);
    RUN_TEST(test_rapidly_changing_layout_type);
    RUN_TEST(test_child_capacity_expansion);
    RUN_TEST(test_padding_larger_than_container);
    RUN_TEST(test_spacing_larger_than_container);
    RUN_TEST(test_hbox_with_no_available_width);
    RUN_TEST(test_vbox_with_no_available_height);
    RUN_TEST(test_deeply_nested_containers);
    RUN_TEST(test_mixed_visible_invisible_children_hbox);
    RUN_TEST(test_mixed_visible_invisible_children_grid);
    RUN_TEST(test_adding_child_to_itself);
    RUN_TEST(test_container_destroy_with_many_children);
    RUN_TEST(test_zero_spacing_and_padding);

    return UNITY_END();
}
