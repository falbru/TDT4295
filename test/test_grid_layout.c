#include "unity.h"
#include "widgets/container.h"
#include "widgets/widget.h"
#include "widgets/label.h"
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

void test_grid_layout_2x2(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(100, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child2->y);
    TEST_ASSERT_EQUAL_INT(0, child3->x);
    TEST_ASSERT_EQUAL_INT(100, child3->y);
    TEST_ASSERT_EQUAL_INT(100, child4->x);
    TEST_ASSERT_EQUAL_INT(100, child4->y);
}

void test_grid_layout_3_columns(void)
{
    container = container_create(0, 0, 300, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 3);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(100, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child2->y);
    TEST_ASSERT_EQUAL_INT(200, child3->x);
    TEST_ASSERT_EQUAL_INT(0, child3->y);
    TEST_ASSERT_EQUAL_INT(0, child4->x);
    TEST_ASSERT_EQUAL_INT(100, child4->y);
}

void test_grid_layout_1_column(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 1);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(0, child2->x);
    TEST_ASSERT_EQUAL_INT(100, child2->y);
    TEST_ASSERT_EQUAL_INT(0, child3->x);
    TEST_ASSERT_EQUAL_INT(200, child3->y);
}

void test_grid_layout_with_spacing(void)
{
    container = container_create(0, 0, 220, 220, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_spacing(container, 10);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(115, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child2->y);
    TEST_ASSERT_EQUAL_INT(0, child3->x);
    TEST_ASSERT_EQUAL_INT(115, child3->y);
    TEST_ASSERT_EQUAL_INT(115, child4->x);
    TEST_ASSERT_EQUAL_INT(115, child4->y);
}

void test_grid_layout_with_padding(void)
{
    container = container_create(0, 0, 240, 240, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_padding(container, 20);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(20, child1->x);
    TEST_ASSERT_EQUAL_INT(20, child1->y);
    TEST_ASSERT_EQUAL_INT(120, child2->x);
    TEST_ASSERT_EQUAL_INT(20, child2->y);
    TEST_ASSERT_EQUAL_INT(20, child3->x);
    TEST_ASSERT_EQUAL_INT(120, child3->y);
    TEST_ASSERT_EQUAL_INT(120, child4->x);
    TEST_ASSERT_EQUAL_INT(120, child4->y);
}

void test_grid_layout_with_padding_and_spacing(void)
{
    container = container_create(0, 0, 250, 250, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_padding(container, 10);
    container_set_spacing(container, 5);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(10, child1->x);
    TEST_ASSERT_EQUAL_INT(10, child1->y);
    TEST_ASSERT_EQUAL_INT(127, child2->x);
    TEST_ASSERT_EQUAL_INT(10, child2->y);
    TEST_ASSERT_EQUAL_INT(10, child3->x);
    TEST_ASSERT_EQUAL_INT(127, child3->y);
}

void test_grid_layout_uneven_children_count(void)
{
    container = container_create(0, 0, 300, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 3);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");
    Widget *child5 = label_create(0, 0, "5");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);
    container_add_child(container, child5);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(100, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child2->y);
    TEST_ASSERT_EQUAL_INT(200, child3->x);
    TEST_ASSERT_EQUAL_INT(0, child3->y);
    TEST_ASSERT_EQUAL_INT(0, child4->x);
    TEST_ASSERT_EQUAL_INT(100, child4->y);
    TEST_ASSERT_EQUAL_INT(100, child5->x);
    TEST_ASSERT_EQUAL_INT(100, child5->y);
}

void test_grid_layout_single_child(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);

    Widget *child = label_create(0, 0, "1");
    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(0, child->y);
    TEST_ASSERT_EQUAL_INT(100, child->width);
    TEST_ASSERT_EQUAL_INT(200, child->height);
}

void test_grid_layout_default_columns(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);

    ContainerData *data = (ContainerData *)container->data;
    TEST_ASSERT_EQUAL_INT(2, data->grid_columns);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(100, child2->x);
}

void test_grid_layout_position_offset(void)
{
    container = container_create(100, 50, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(100, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child1->y);
    TEST_ASSERT_EQUAL_INT(200, child2->x);
    TEST_ASSERT_EQUAL_INT(50, child2->y);
}

void test_grid_layout_cell_sizes_uniform(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");
    Widget *child4 = label_create(0, 0, "4");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);
    container_add_child(container, child4);

    TEST_ASSERT_EQUAL_INT(100, child1->width);
    TEST_ASSERT_EQUAL_INT(100, child1->height);
    TEST_ASSERT_EQUAL_INT(100, child2->width);
    TEST_ASSERT_EQUAL_INT(100, child2->height);
    TEST_ASSERT_EQUAL_INT(100, child3->width);
    TEST_ASSERT_EQUAL_INT(100, child3->height);
    TEST_ASSERT_EQUAL_INT(100, child4->width);
    TEST_ASSERT_EQUAL_INT(100, child4->height);
}

void test_grid_layout_updates_when_columns_change(void)
{
    container = container_create(0, 0, 300, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);

    Widget *child1 = label_create(0, 0, "1");
    Widget *child2 = label_create(0, 0, "2");
    Widget *child3 = label_create(0, 0, "3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(150, child2->x);
    TEST_ASSERT_EQUAL_INT(0, child3->x);
    TEST_ASSERT_EQUAL_INT(100, child3->y);

    container_set_grid_columns(container, 3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(100, child2->x);
    TEST_ASSERT_EQUAL_INT(200, child3->x);
    TEST_ASSERT_EQUAL_INT(0, child3->y);
}

void test_grid_layout_with_many_children(void)
{
    container = container_create(0, 0, 400, 400, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 4);

    Widget *children[16];
    for (int i = 0; i < 16; i++)
    {
        children[i] = label_create(0, 0, "X");
        container_add_child(container, children[i]);
    }

    for (int i = 0; i < 16; i++)
    {
        int row = i / 4;
        int col = i % 4;
        TEST_ASSERT_EQUAL_INT(col * 100, children[i]->x);
        TEST_ASSERT_EQUAL_INT(row * 100, children[i]->y);
    }
}

void test_grid_layout_center_alignment(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "1");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(25, child->x);
    TEST_ASSERT_EQUAL_INT(75, child->y);
    TEST_ASSERT_EQUAL_INT(50, child->width);
    TEST_ASSERT_EQUAL_INT(50, child->height);
}

void test_grid_layout_stretch_alignment(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_GRID);
    container_set_grid_columns(container, 2);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child = label_create(0, 0, "1");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(100, child->width);
    TEST_ASSERT_EQUAL_INT(200, child->height);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_grid_layout_2x2);
    RUN_TEST(test_grid_layout_3_columns);
    RUN_TEST(test_grid_layout_1_column);
    RUN_TEST(test_grid_layout_with_spacing);
    RUN_TEST(test_grid_layout_with_padding);
    RUN_TEST(test_grid_layout_with_padding_and_spacing);
    RUN_TEST(test_grid_layout_uneven_children_count);
    RUN_TEST(test_grid_layout_single_child);
    RUN_TEST(test_grid_layout_default_columns);
    RUN_TEST(test_grid_layout_position_offset);
    RUN_TEST(test_grid_layout_cell_sizes_uniform);
    RUN_TEST(test_grid_layout_updates_when_columns_change);
    RUN_TEST(test_grid_layout_with_many_children);
    RUN_TEST(test_grid_layout_center_alignment);
    RUN_TEST(test_grid_layout_stretch_alignment);

    return UNITY_END();
}
