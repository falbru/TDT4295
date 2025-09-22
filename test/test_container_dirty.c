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

void test_container_dirty_on_creation(void)
{
    container = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_child_dirty_propagates_to_parent(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_mark_dirty(child);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_multiple_children_dirty_propagates(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");
    Widget *child3 = label_create(0, 0, "Child 3");

    container_add_child(container, child1);
    container_add_child(container, child2);
    container_add_child(container, child3);

    container->dirty = false;
    child1->dirty = false;
    child2->dirty = false;
    child3->dirty = false;

    widget_mark_dirty(child2);

    TEST_ASSERT_FALSE(child1->dirty);
    TEST_ASSERT_TRUE(child2->dirty);
    TEST_ASSERT_FALSE(child3->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_nested_container_dirty_propagation(void)
{
    container = container_create(0, 0, 300, 300, LAYOUT_TYPE_VBOX);
    Widget *nested_container = container_create(0, 0, 200, 100, LAYOUT_TYPE_HBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, nested_container);
    container_add_child(nested_container, child);

    container->dirty = false;
    nested_container->dirty = false;
    child->dirty = false;

    widget_mark_dirty(child);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(nested_container->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_adding_child_marks_container_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    container->dirty = false;

    Widget *child = label_create(0, 0, "Child");
    container_add_child(container, child);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_removing_child_marks_container_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);
    container->dirty = false;

    Widget *child2 = label_create(0, 0, "Child 2");
    container_add_child(container, child2);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_set_position_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(10, 10, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_position(child, 20, 20);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_set_size_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_size(child, 60, 60);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_set_visible_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_visible(child, false);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_set_position_same_value_does_not_mark_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_NONE);
    Widget *child = label_create(10, 20, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_position(child, 10, 20);

    TEST_ASSERT_FALSE(child->dirty);
    TEST_ASSERT_FALSE(container->dirty);
}

void test_set_size_same_value_does_not_mark_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");
    widget_set_size(child, 50, 50);

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_size(child, 50, 50);

    TEST_ASSERT_FALSE(child->dirty);
    TEST_ASSERT_FALSE(container->dirty);
}

void test_set_visible_same_value_does_not_mark_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_set_visible(child, true);

    TEST_ASSERT_FALSE(child->dirty);
    TEST_ASSERT_FALSE(container->dirty);
}

void test_container_set_spacing_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    container->dirty = false;

    container_set_spacing(container, 10);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_set_padding_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    container->dirty = false;

    container_set_padding(container, 10);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_set_alignment_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    container->dirty = false;

    container_set_alignment(container, ALIGN_CENTER);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_set_justify_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    container->dirty = false;

    container_set_justify(container, ALIGN_CENTER);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_clear_children_marks_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);

    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");

    container_add_child(container, child1);
    container_add_child(container, child2);

    container->dirty = false;

    container_clear_children(container);

    TEST_ASSERT_TRUE(container->dirty);
}

void test_deeply_nested_dirty_propagation(void)
{
    container = container_create(0, 0, 400, 400, LAYOUT_TYPE_VBOX);
    Widget *level1 = container_create(0, 0, 300, 300, LAYOUT_TYPE_VBOX);
    Widget *level2 = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *level3 = container_create(0, 0, 100, 100, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Deep Child");

    container_add_child(container, level1);
    container_add_child(level1, level2);
    container_add_child(level2, level3);
    container_add_child(level3, child);

    container->dirty = false;
    level1->dirty = false;
    level2->dirty = false;
    level3->dirty = false;
    child->dirty = false;

    widget_mark_dirty(child);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(level3->dirty);
    TEST_ASSERT_TRUE(level2->dirty);
    TEST_ASSERT_TRUE(level1->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_container_has_dirty_callback(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);

    TEST_ASSERT_NOT_NULL(container->on_dirty);
}

void test_sibling_dirty_does_not_affect_each_other(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child1 = label_create(0, 0, "Child 1");
    Widget *child2 = label_create(0, 0, "Child 2");

    container_add_child(container, child1);
    container_add_child(container, child2);

    container->dirty = false;
    child1->dirty = false;
    child2->dirty = false;

    widget_mark_dirty(child1);

    TEST_ASSERT_TRUE(child1->dirty);
    TEST_ASSERT_FALSE(child2->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

void test_widget_mark_dirty_on_null_widget(void)
{
    widget_mark_dirty(NULL);
}

void test_multiple_dirty_marks_stay_dirty(void)
{
    container = container_create(0, 0, 200, 200, LAYOUT_TYPE_VBOX);
    Widget *child = label_create(0, 0, "Child");

    container_add_child(container, child);

    container->dirty = false;
    child->dirty = false;

    widget_mark_dirty(child);
    widget_mark_dirty(child);
    widget_mark_dirty(child);

    TEST_ASSERT_TRUE(child->dirty);
    TEST_ASSERT_TRUE(container->dirty);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_container_dirty_on_creation);
    RUN_TEST(test_child_dirty_propagates_to_parent);
    RUN_TEST(test_multiple_children_dirty_propagates);
    RUN_TEST(test_nested_container_dirty_propagation);
    RUN_TEST(test_adding_child_marks_container_dirty);
    RUN_TEST(test_removing_child_marks_container_dirty);
    RUN_TEST(test_set_position_marks_dirty);
    RUN_TEST(test_set_size_marks_dirty);
    RUN_TEST(test_set_visible_marks_dirty);
    RUN_TEST(test_set_position_same_value_does_not_mark_dirty);
    RUN_TEST(test_set_size_same_value_does_not_mark_dirty);
    RUN_TEST(test_set_visible_same_value_does_not_mark_dirty);
    RUN_TEST(test_container_set_spacing_marks_dirty);
    RUN_TEST(test_container_set_padding_marks_dirty);
    RUN_TEST(test_container_set_alignment_marks_dirty);
    RUN_TEST(test_container_set_justify_marks_dirty);
    RUN_TEST(test_container_clear_children_marks_dirty);
    RUN_TEST(test_deeply_nested_dirty_propagation);
    RUN_TEST(test_container_has_dirty_callback);
    RUN_TEST(test_sibling_dirty_does_not_affect_each_other);
    RUN_TEST(test_widget_mark_dirty_on_null_widget);
    RUN_TEST(test_multiple_dirty_marks_stay_dirty);

    return UNITY_END();
}
