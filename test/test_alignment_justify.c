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

void test_hbox_alignment_start_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_START);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 100);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->y);
    TEST_ASSERT_EQUAL_INT(100, child->height);
}

void test_hbox_alignment_center_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 100);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(50, child->y);
    TEST_ASSERT_EQUAL_INT(100, child->height);
}

void test_hbox_alignment_end_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 100);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(100, child->y);
    TEST_ASSERT_EQUAL_INT(100, child->height);
}

void test_hbox_alignment_stretch_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 100);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->y);
    TEST_ASSERT_EQUAL_INT(200, child->height);
}

void test_hbox_justify_start_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_START);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
}

void test_hbox_justify_center_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(75, child->x);
}

void test_hbox_justify_end_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(150, child->x);
}

void test_hbox_justify_center_multiple_children(void)
{
    container = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_CENTER);
    container_set_spacing(container, 10);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    int total_content_width = 50 + 10 + 50 + 10 + 50;
    int start_x = (400 - total_content_width) / 2;

    TEST_ASSERT_EQUAL_INT(start_x, child1->x);
    TEST_ASSERT_EQUAL_INT(start_x + 60, child2->x);
    TEST_ASSERT_EQUAL_INT(start_x + 120, child3->x);
}

void test_hbox_justify_end_multiple_children(void)
{
    container = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_END);
    container_set_spacing(container, 10);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    int total_content_width = 50 + 10 + 50;
    int start_x = 400 - total_content_width;

    TEST_ASSERT_EQUAL_INT(start_x, child1->x);
    TEST_ASSERT_EQUAL_INT(start_x + 60, child2->x);
}

void test_vbox_alignment_start_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_START);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 100, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(100, child->width);
}

void test_vbox_alignment_center_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 100, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(50, child->x);
    TEST_ASSERT_EQUAL_INT(100, child->width);
}

void test_vbox_alignment_end_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 100, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(100, child->x);
    TEST_ASSERT_EQUAL_INT(100, child->width);
}

void test_vbox_alignment_stretch_single_child(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 100, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(200, child->width);
}

void test_vbox_justify_start_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_START);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->y);
}

void test_vbox_justify_center_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(75, child->y);
}

void test_vbox_justify_end_single_child(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(150, child->y);
}

void test_vbox_justify_center_multiple_children(void)
{
    container = container_create(0, 0, 100, 400, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_CENTER);
    container_set_spacing(container, 10);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    int total_content_height = 50 + 10 + 50 + 10 + 50;
    int start_y = (400 - total_content_height) / 2;

    TEST_ASSERT_EQUAL_INT(start_y, child1->y);
    TEST_ASSERT_EQUAL_INT(start_y + 60, child2->y);
    TEST_ASSERT_EQUAL_INT(start_y + 120, child3->y);
}

void test_vbox_justify_end_multiple_children(void)
{
    container = container_create(0, 0, 100, 400, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_END);
    container_set_spacing(container, 10);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    int total_content_height = 50 + 10 + 50;
    int start_y = 400 - total_content_height;

    TEST_ASSERT_EQUAL_INT(start_y, child1->y);
    TEST_ASSERT_EQUAL_INT(start_y + 60, child2->y);
}

void test_hbox_alignment_with_padding(void)
{
    container = container_create(0, 0, 100, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_CENTER);
    container_set_padding(container, 20);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 100);

    container_add_child(container, child);

    int available_height = 200 - 40;
    int expected_y = 20 + (available_height - 100) / 2;

    TEST_ASSERT_EQUAL_INT(expected_y, child->y);
}

void test_vbox_alignment_with_padding(void)
{
    container = container_create(0, 0, 200, 100, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_CENTER);
    container_set_padding(container, 20);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 100, 50);

    container_add_child(container, child);

    int available_width = 200 - 40;
    int expected_x = 20 + (available_width - 100) / 2;

    TEST_ASSERT_EQUAL_INT(expected_x, child->x);
}

void test_alignment_and_justify_combination_hbox(void)
{
    container = container_create(0, 0, 400, 200, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_CENTER);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(175, child->x);
    TEST_ASSERT_EQUAL_INT(75, child->y);
}

void test_alignment_and_justify_combination_vbox(void)
{
    container = container_create(0, 0, 200, 400, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_CENTER);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(75, child->x);
    TEST_ASSERT_EQUAL_INT(175, child->y);
}

void test_hbox_alignment_multiple_children_different_sizes(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 30);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 40);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(35, child1->y);
    TEST_ASSERT_EQUAL_INT(25, child2->y);
    TEST_ASSERT_EQUAL_INT(30, child3->y);
}

void test_vbox_alignment_multiple_children_different_sizes(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 30, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 40, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(35, child1->x);
    TEST_ASSERT_EQUAL_INT(25, child2->x);
    TEST_ASSERT_EQUAL_INT(30, child3->x);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_hbox_alignment_start_single_child);
    RUN_TEST(test_hbox_alignment_center_single_child);
    RUN_TEST(test_hbox_alignment_end_single_child);
    RUN_TEST(test_hbox_alignment_stretch_single_child);
    RUN_TEST(test_hbox_justify_start_single_child);
    RUN_TEST(test_hbox_justify_center_single_child);
    RUN_TEST(test_hbox_justify_end_single_child);
    RUN_TEST(test_hbox_justify_center_multiple_children);
    RUN_TEST(test_hbox_justify_end_multiple_children);

    RUN_TEST(test_vbox_alignment_start_single_child);
    RUN_TEST(test_vbox_alignment_center_single_child);
    RUN_TEST(test_vbox_alignment_end_single_child);
    RUN_TEST(test_vbox_alignment_stretch_single_child);
    RUN_TEST(test_vbox_justify_start_single_child);
    RUN_TEST(test_vbox_justify_center_single_child);
    RUN_TEST(test_vbox_justify_end_single_child);
    RUN_TEST(test_vbox_justify_center_multiple_children);
    RUN_TEST(test_vbox_justify_end_multiple_children);

    RUN_TEST(test_hbox_alignment_with_padding);
    RUN_TEST(test_vbox_alignment_with_padding);
    RUN_TEST(test_alignment_and_justify_combination_hbox);
    RUN_TEST(test_alignment_and_justify_combination_vbox);
    RUN_TEST(test_hbox_alignment_multiple_children_different_sizes);
    RUN_TEST(test_vbox_alignment_multiple_children_different_sizes);

    return UNITY_END();
}
