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

void test_hbox_layout_basic_positioning(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 30);
    widget_set_size(child2, 60, 40);
    widget_set_size(child3, 70, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child2->x);
    TEST_ASSERT_EQUAL_INT(110, child3->x);
}

void test_hbox_layout_with_spacing(void)
{
    container = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);
    container_set_spacing(container, 15);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(65, child2->x);
    TEST_ASSERT_EQUAL_INT(130, child3->x);
}

void test_hbox_layout_with_padding(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_padding(container, 20);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(20, child->x);
    TEST_ASSERT_EQUAL_INT(20, child->y);
}

void test_hbox_layout_center_alignment(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 30);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(35, child->y);
}

void test_hbox_layout_end_alignment(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 30);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(70, child->y);
}

void test_hbox_layout_stretch_alignment(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 30);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->y);
    TEST_ASSERT_EQUAL_INT(100, child->height);
}

void test_hbox_layout_center_justify(void)
{
    container = container_create(100, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(225, child->x);
}

void test_hbox_layout_end_justify(void)
{
    container = container_create(100, 0, 300, 100, LAYOUT_TYPE_HBOX);
    container_set_justify(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(350, child->x);
}

void test_vbox_layout_basic_positioning(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 30, 50);
    widget_set_size(child2, 40, 60);
    widget_set_size(child3, 50, 70);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(50, child2->y);
    TEST_ASSERT_EQUAL_INT(110, child3->y);
}

void test_vbox_layout_with_spacing(void)
{
    container = container_create(0, 0, 100, 400, LAYOUT_TYPE_VBOX);
    container_set_spacing(container, 15);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(65, child2->y);
    TEST_ASSERT_EQUAL_INT(130, child3->y);
}

void test_vbox_layout_with_padding(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_padding(container, 20);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(20, child->x);
    TEST_ASSERT_EQUAL_INT(20, child->y);
}

void test_vbox_layout_center_alignment(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 30, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(35, child->x);
}

void test_vbox_layout_end_alignment(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 30, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(70, child->x);
}

void test_vbox_layout_stretch_alignment(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_alignment(container, ALIGN_STRETCH);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 30, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(100, child->width);
}

void test_vbox_layout_center_justify(void)
{
    container = container_create(0, 100, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_CENTER);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(225, child->y);
}

void test_vbox_layout_end_justify(void)
{
    container = container_create(0, 100, 100, 300, LAYOUT_TYPE_VBOX);
    container_set_justify(container, ALIGN_END);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(350, child->y);
}

void test_hbox_layout_with_invisible_children(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    widget_set_visible(child2, false);
    container_update_layout(container);

    TEST_ASSERT_EQUAL_INT(0, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child3->x);
}

void test_vbox_layout_with_invisible_children(void)
{
    container = container_create(0, 0, 100, 300, LAYOUT_TYPE_VBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");
    Widget *child3 = label_create(0, 0, "C");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);
    widget_set_size(child3, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    widget_set_visible(child2, false);
    container_update_layout(container);

    TEST_ASSERT_EQUAL_INT(0, child1->y);
    TEST_ASSERT_EQUAL_INT(50, child3->y);
}

void test_layout_updates_when_spacing_changes(void)
{
    container = container_create(0, 0, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(50, child2->x);

    container_set_spacing(container, 10);

    TEST_ASSERT_EQUAL_INT(60, child2->x);
}

void test_layout_updates_when_padding_changes(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);
    TEST_ASSERT_EQUAL_INT(0, child->y);

    container_set_padding(container, 10);

    TEST_ASSERT_EQUAL_INT(10, child->x);
    TEST_ASSERT_EQUAL_INT(10, child->y);
}

void test_layout_updates_when_alignment_changes(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);

    Widget *child = label_create(0, 0, "A");
    widget_set_size(child, 30, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(0, child->x);

    container_set_alignment(container, ALIGN_CENTER);

    TEST_ASSERT_EQUAL_INT(35, child->x);
}

void test_hbox_with_padding_and_spacing_combined(void)
{
    container = container_create(0, 0, 400, 100, LAYOUT_TYPE_HBOX);
    container_set_padding(container, 10);
    container_set_spacing(container, 5);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(10, child1->x);
    TEST_ASSERT_EQUAL_INT(10, child1->y);
    TEST_ASSERT_EQUAL_INT(65, child2->x);
    TEST_ASSERT_EQUAL_INT(10, child2->y);
}

void test_vbox_with_padding_and_spacing_combined(void)
{
    container = container_create(0, 0, 100, 400, LAYOUT_TYPE_VBOX);
    container_set_padding(container, 10);
    container_set_spacing(container, 5);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(10, child1->x);
    TEST_ASSERT_EQUAL_INT(10, child1->y);
    TEST_ASSERT_EQUAL_INT(10, child2->x);
    TEST_ASSERT_EQUAL_INT(65, child2->y);
}

void test_container_position_offset_applies_to_children(void)
{
    container = container_create(100, 50, 300, 100, LAYOUT_TYPE_HBOX);

    Widget *child1 = label_create(0, 0, "A");
    Widget *child2 = label_create(0, 0, "B");

    widget_set_size(child1, 50, 50);
    widget_set_size(child2, 50, 50);

    container_add_child(container, child1);
    container_add_child(container, child2);

    TEST_ASSERT_EQUAL_INT(100, child1->x);
    TEST_ASSERT_EQUAL_INT(50, child1->y);
    TEST_ASSERT_EQUAL_INT(150, child2->x);
    TEST_ASSERT_EQUAL_INT(50, child2->y);
}

void test_layout_none_does_not_modify_children(void)
{
    container = container_create(0, 0, 300, 300, LAYOUT_TYPE_NONE);

    Widget *child = label_create(25, 30, "A");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    TEST_ASSERT_EQUAL_INT(25, child->x);
    TEST_ASSERT_EQUAL_INT(30, child->y);
    TEST_ASSERT_EQUAL_INT(50, child->width);
    TEST_ASSERT_EQUAL_INT(50, child->height);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_hbox_layout_basic_positioning);
    RUN_TEST(test_hbox_layout_with_spacing);
    RUN_TEST(test_hbox_layout_with_padding);
    RUN_TEST(test_hbox_layout_center_alignment);
    RUN_TEST(test_hbox_layout_end_alignment);
    RUN_TEST(test_hbox_layout_stretch_alignment);
    RUN_TEST(test_hbox_layout_center_justify);
    RUN_TEST(test_hbox_layout_end_justify);

    RUN_TEST(test_vbox_layout_basic_positioning);
    RUN_TEST(test_vbox_layout_with_spacing);
    RUN_TEST(test_vbox_layout_with_padding);
    RUN_TEST(test_vbox_layout_center_alignment);
    RUN_TEST(test_vbox_layout_end_alignment);
    RUN_TEST(test_vbox_layout_stretch_alignment);
    RUN_TEST(test_vbox_layout_center_justify);
    RUN_TEST(test_vbox_layout_end_justify);

    RUN_TEST(test_hbox_layout_with_invisible_children);
    RUN_TEST(test_vbox_layout_with_invisible_children);

    RUN_TEST(test_layout_updates_when_spacing_changes);
    RUN_TEST(test_layout_updates_when_padding_changes);
    RUN_TEST(test_layout_updates_when_alignment_changes);

    RUN_TEST(test_hbox_with_padding_and_spacing_combined);
    RUN_TEST(test_vbox_with_padding_and_spacing_combined);

    RUN_TEST(test_container_position_offset_applies_to_children);
    RUN_TEST(test_layout_none_does_not_modify_children);

    return UNITY_END();
}
