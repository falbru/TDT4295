#include "unity.h"
#include "widgets/container.h"
#include "widgets/widget.h"
#include "widgets/label.h"
#include "widgets/button.h"
#include <stdlib.h>

static Widget *container;

void setUp(void)
{
    container = container_create(10, 20, 200, 150, LAYOUT_TYPE_VBOX);
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

void test_container_creation(void)
{
    TEST_ASSERT_NOT_NULL(container);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_CONTAINER, container->type);
    TEST_ASSERT_EQUAL_INT(10, container->x);
    TEST_ASSERT_EQUAL_INT(20, container->y);
    TEST_ASSERT_EQUAL_INT(200, container->width);
    TEST_ASSERT_EQUAL_INT(150, container->height);
    TEST_ASSERT_TRUE(container->visible);
    TEST_ASSERT_TRUE(container->enabled);
    TEST_ASSERT_NOT_NULL(container->data);
}

void test_container_initial_child_count(void)
{
    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(container));
}

void test_container_default_layout_type(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(LAYOUT_TYPE_VBOX, data->layout_type);
}

void test_container_default_spacing(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(0, data->spacing);
}

void test_container_default_padding(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(0, data->padding);
}

void test_container_default_alignment(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(ALIGN_START, data->alignment);
}

void test_container_default_justify(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(ALIGN_START, data->justify);
}

void test_container_default_grid_columns(void)
{
    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(2, data->grid_columns);
}

void test_container_add_child(void)
{
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(1, container_get_child_count(container));
    TEST_ASSERT_EQUAL_PTR(child, container_get_child(container, 0));
    TEST_ASSERT_EQUAL_PTR(container, child->parent);
}

void test_container_add_multiple_children(void)
{
    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");
    Widget *child3 = label_create(0, 0, "Child 3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(3, container_get_child_count(container));
    TEST_ASSERT_EQUAL_PTR(child1, container_get_child(container, 0));
    TEST_ASSERT_EQUAL_PTR(child2, container_get_child(container, 1));
    TEST_ASSERT_EQUAL_PTR(child3, container_get_child(container, 2));
}

void test_container_add_child_sets_parent(void)
{
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_PTR(container, child->parent);
}

void test_container_add_child_with_null_container(void)
{
    Widget *child = label_create(0, 0, "Child");

    container_add_child(NULL, child);

    widget_destroy(child);
    free(child);
}

void test_container_add_child_with_null_child(void)
{
    container_add_child(container, NULL);

    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(container));
}

void test_container_remove_child(void)
{
    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");
    Widget *child3 = label_create(0, 0, "Child 3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    container_remove_child(container, child2);

    TEST_ASSERT_EQUAL_INT(2, container_get_child_count(container));
    TEST_ASSERT_EQUAL_PTR(child1, container_get_child(container, 0));
    TEST_ASSERT_EQUAL_PTR(child3, container_get_child(container, 1));
}

void test_container_remove_child_with_null_container(void)
{
    Widget *child = label_create(0, 0, "Child");

    container_remove_child(NULL, child);

    widget_destroy(child);
    free(child);
}

void test_container_remove_child_with_null_child(void)
{
    container_remove_child(container, NULL);
}

void test_container_clear_children(void)
{
    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");
    Widget *child3 = label_create(0, 0, "Child 3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    container_clear_children(container);

    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(container));
}

void test_container_clear_children_with_null(void)
{
    container_clear_children(NULL);
}

void test_container_set_spacing(void)
{
    container_set_spacing(container, 10);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(10, data->spacing);
}

void test_container_set_spacing_with_null(void)
{
    container_set_spacing(NULL, 10);
}

void test_container_set_padding(void)
{
    container_set_padding(container, 15);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(15, data->padding);
}

void test_container_set_padding_with_null(void)
{
    container_set_padding(NULL, 15);
}

void test_container_set_alignment(void)
{
    container_set_alignment(container, ALIGN_CENTER);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(ALIGN_CENTER, data->alignment);
}

void test_container_set_alignment_with_null(void)
{
    container_set_alignment(NULL, ALIGN_CENTER);
}

void test_container_set_justify(void)
{
    container_set_justify(container, ALIGN_END);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(ALIGN_END, data->justify);
}

void test_container_set_justify_with_null(void)
{
    container_set_justify(NULL, ALIGN_END);
}

void test_container_set_grid_columns(void)
{
    container_set_grid_columns(container, 3);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(3, data->grid_columns);
}

void test_container_set_grid_columns_zero_sets_to_one(void)
{
    container_set_grid_columns(container, 0);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(1, data->grid_columns);
}

void test_container_set_grid_columns_negative_sets_to_one(void)
{
    container_set_grid_columns(container, -5);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(1, data->grid_columns);
}

void test_container_set_grid_columns_with_null(void)
{
    container_set_grid_columns(NULL, 3);
}

void test_container_get_child_count_with_null(void)
{
    TEST_ASSERT_EQUAL_INT(0, container_get_child_count(NULL));
}

void test_container_get_child_with_null_container(void)
{
    TEST_ASSERT_NULL(container_get_child(NULL, 0));
}

void test_container_get_child_with_invalid_index(void)
{
    TEST_ASSERT_NULL(container_get_child(container, -1));
    TEST_ASSERT_NULL(container_get_child(container, 0));
    TEST_ASSERT_NULL(container_get_child(container, 100));
}

void test_container_get_child_with_valid_index(void)
{
    Widget *child = label_create(0, 0, "Child");
    container_add_child(container, child);

    TEST_ASSERT_EQUAL_PTR(child, container_get_child(container, 0));
}

void test_container_has_render_callback(void)
{
    TEST_ASSERT_NOT_NULL(container->render);
}

void test_container_has_destroy_callback(void)
{
    TEST_ASSERT_NOT_NULL(container->destroy);
}

void test_container_has_dirty_callback(void)
{
    TEST_ASSERT_NOT_NULL(container->on_dirty);
}

void test_container_dirty_flag_on_creation(void)
{
    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_child_dirty_propagates_to_container(void)
{
    Widget *child = label_create(0, 0, "Child");
    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_mark_dirty(child);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_update_layout_with_null(void)
{
    container_update_layout(NULL);
}

void test_container_hbox_layout(void)
{
    Widget *hbox = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(hbox, child1);
    container_add_child(hbox, child2);
    container_add_child(hbox, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child2->x);
    TEST_ASSERT_EQUAL_INT(100, child3->x);

    widget_destroy(hbox);
    free(hbox);
}

void test_container_vbox_layout(void)
{
    Widget *vbox = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(vbox, child1);
    container_add_child(vbox, child2);
    container_add_child(vbox, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(50, child2->y);
    TEST_ASSERT_EQUAL_INT(100, child3->y);

    widget_destroy(vbox);
    free(vbox);
}

void test_container_hbox_with_spacing(void)
{
    Widget *hbox = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);
    container_set_spacing(hbox, 10);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(hbox, child1);
    container_add_child(hbox, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(60, child2->x);

    widget_destroy(hbox);
    free(hbox);
}

void test_container_vbox_with_padding(void)
{
    Widget *vbox = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_padding(vbox, 10);

    Widget *child = label_create(0, 0, "1");
    widget_set_size(child, 50, 50);

    container_add_child(vbox, child);

    TEST_ASSERT_EQUAL_INT(10, child->x);
    TEST_ASSERT_EQUAL_INT(10, child->y);

    widget_destroy(vbox);
    free(vbox);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_container_creation);
    RUN_TEST(test_container_initial_child_count);
    RUN_TEST(test_container_default_layout_type);
    RUN_TEST(test_container_default_spacing);
    RUN_TEST(test_container_default_padding);
    RUN_TEST(test_container_default_alignment);
    RUN_TEST(test_container_default_justify);
    RUN_TEST(test_container_default_grid_columns);
    RUN_TEST(test_container_add_child);
    RUN_TEST(test_container_add_multiple_children);
    RUN_TEST(test_container_add_child_sets_parent);
    RUN_TEST(test_container_add_child_with_null_container);
    RUN_TEST(test_container_add_child_with_null_child);
    RUN_TEST(test_container_remove_child);
    RUN_TEST(test_container_remove_child_with_null_container);
    RUN_TEST(test_container_remove_child_with_null_child);
    RUN_TEST(test_container_clear_children);
    RUN_TEST(test_container_clear_children_with_null);
    RUN_TEST(test_container_set_spacing);
    RUN_TEST(test_container_set_spacing_with_null);
    RUN_TEST(test_container_set_padding);
    RUN_TEST(test_container_set_padding_with_null);
    RUN_TEST(test_container_set_alignment);
    RUN_TEST(test_container_set_alignment_with_null);
    RUN_TEST(test_container_set_justify);
    RUN_TEST(test_container_set_justify_with_null);
    RUN_TEST(test_container_set_grid_columns);
    RUN_TEST(test_container_set_grid_columns_zero_sets_to_one);
    RUN_TEST(test_container_set_grid_columns_negative_sets_to_one);
    RUN_TEST(test_container_set_grid_columns_with_null);
    RUN_TEST(test_container_get_child_count_with_null);
    RUN_TEST(test_container_get_child_with_null_container);
    RUN_TEST(test_container_get_child_with_invalid_index);
    RUN_TEST(test_container_get_child_with_valid_index);
    RUN_TEST(test_container_has_render_callback);
    RUN_TEST(test_container_has_destroy_callback);
    RUN_TEST(test_container_has_dirty_callback);
    RUN_TEST(test_container_dirty_flag_on_creation);
    RUN_TEST(test_container_child_dirty_propagates_to_container);
    RUN_TEST(test_container_update_layout_with_null);
    RUN_TEST(test_container_hbox_layout);
    RUN_TEST(test_container_vbox_layout);
    RUN_TEST(test_container_hbox_with_spacing);
    RUN_TEST(test_container_vbox_with_padding);

    return UNITY_END();
}
