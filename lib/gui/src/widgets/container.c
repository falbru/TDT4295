#include "widgets/container.h"
#include <stdlib.h>
#include <string.h>

static void container_render_callback(Widget *widget, Framebuffer *framebuffer);
static void container_destroy_callback(Widget *widget);
static void update_hbox_layout(Widget *container);
static void update_vbox_layout(Widget *container);
static void update_grid_layout(Widget *container);

Widget *container_create(int x, int y, int width, int height, LayoutType layout_type)
{
    Widget *container = (Widget *)malloc(sizeof(Widget));
    if (!container)
        return NULL;

    widget_init(container, WIDGET_TYPE_CONTAINER, x, y, width, height);

    ContainerData *data = (ContainerData *)malloc(sizeof(ContainerData));
    if (!data)
    {
        free(container);
        return NULL;
    }

    data->child_capacity = 8;
    data->children = (Widget **)malloc(sizeof(Widget *) * data->child_capacity);
    if (!data->children)
    {
        free(data);
        free(container);
        return NULL;
    }

    data->child_count = 0;
    data->layout_type = layout_type;
    data->spacing = 0;
    data->padding = 0;
    data->alignment = ALIGN_START;
    data->justify = ALIGN_START;
    data->grid_columns = 2;

    container->data = data;
    container->render = container_render_callback;
    container->destroy = container_destroy_callback;

    return container;
}

static void container_render_callback(Widget *widget, Framebuffer *framebuffer)
{
    if (!widget || !widget->data)
        return;

    ContainerData *data = (ContainerData *)widget->data;

    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i])
        {
            widget_render(data->children[i], framebuffer);
        }
    }
}

static void container_destroy_callback(Widget *widget)
{
    if (!widget || !widget->data)
        return;

    ContainerData *data = (ContainerData *)widget->data;

    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i])
        {
            widget_destroy(data->children[i]);
            free(data->children[i]);
            data->children[i] = NULL;
        }
    }

    if (data->children)
    {
        free(data->children);
        data->children = NULL;
    }
}

void container_add_child(Widget *container, Widget *child)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER || !child)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    if (data->child_count >= data->child_capacity)
    {
        int new_capacity = data->child_capacity * 2;
        Widget **new_children = (Widget **)realloc(data->children, sizeof(Widget *) * new_capacity);
        if (!new_children)
            return;

        data->children = new_children;
        data->child_capacity = new_capacity;
    }

    data->children[data->child_count] = child;
    data->child_count++;

    child->parent = container;

    container_update_layout(container);
}

void container_remove_child(Widget *container, Widget *child)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER || !child)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i] == child)
        {
            child->parent = NULL;

            widget_destroy(child);
            free(child);

            for (int j = i; j < data->child_count - 1; j++)
            {
                data->children[j] = data->children[j + 1];
            }

            data->child_count--;
            container_update_layout(container);
            return;
        }
    }
}

void container_clear_children(Widget *container)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i])
        {
            data->children[i]->parent = NULL;

            widget_destroy(data->children[i]);
            free(data->children[i]);
            data->children[i] = NULL;
        }
    }

    data->child_count = 0;
    container_update_layout(container);
}

void container_set_spacing(Widget *container, int spacing)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    data->spacing = spacing;
    container_update_layout(container);
}

void container_set_padding(Widget *container, int padding)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    data->padding = padding;
    container_update_layout(container);
}

void container_set_alignment(Widget *container, Alignment alignment)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    data->alignment = alignment;
    container_update_layout(container);
}

void container_set_justify(Widget *container, Alignment justify)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    data->justify = justify;
    container_update_layout(container);
}

void container_set_grid_columns(Widget *container, int columns)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    data->grid_columns = columns > 0 ? columns : 1;
    container_update_layout(container);
}

void container_update_layout(Widget *container)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return;

    switch (data->layout_type)
    {
    case LAYOUT_TYPE_HBOX:
        update_hbox_layout(container);
        break;
    case LAYOUT_TYPE_VBOX:
        update_vbox_layout(container);
        break;
    case LAYOUT_TYPE_GRID:
        update_grid_layout(container);
        break;
    }
}

static void update_hbox_layout(Widget *container)
{
    ContainerData *data = (ContainerData *)container->data;
    if (!data || data->child_count == 0)
        return;

    int visible_count = 0;
    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i]->visible)
        {
            visible_count++;
        }
    }

    if (visible_count == 0)
        return;

    int available_width = container->width - (2 * data->padding) - ((visible_count - 1) * data->spacing);
    int available_height = container->height - (2 * data->padding);

    int total_fixed_width = 0;
    int flexible_count = 0;

    for (int i = 0; i < data->child_count; i++)
    {
        Widget *child = data->children[i];
        if (!child->visible)
            continue;

        if (child->width > 0)
        {
            total_fixed_width += child->width;
        }
        else
        {
            flexible_count++;
        }
    }

    int flexible_width = flexible_count > 0 ? (available_width - total_fixed_width) / flexible_count : 0;

    int total_content_width =
        total_fixed_width + (flexible_count * flexible_width) + ((visible_count - 1) * data->spacing);

    int current_x = container->x + data->padding;
    if (data->justify == ALIGN_CENTER && total_content_width < available_width)
    {
        current_x = container->x + (container->width - total_content_width) / 2;
    }
    else if (data->justify == ALIGN_END && total_content_width < available_width)
    {
        current_x = container->x + container->width - data->padding - total_content_width;
    }

    for (int i = 0; i < data->child_count; i++)
    {
        Widget *child = data->children[i];
        if (!child->visible)
            continue;

        int child_width = child->width > 0 ? child->width : flexible_width;
        int child_height;
        int child_y;

        if (data->alignment == ALIGN_STRETCH)
        {
            child_height = available_height;
            child_y = container->y + data->padding;
        }
        else
        {
            child_height = child->height > 0 ? child->height : available_height;

            if (data->alignment == ALIGN_CENTER)
            {
                child_y = container->y + data->padding + (available_height - child_height) / 2;
            }
            else if (data->alignment == ALIGN_END)
            {
                child_y = container->y + data->padding + available_height - child_height;
            }
            else // ALIGN_START
            {
                child_y = container->y + data->padding;
            }
        }

        widget_set_position(child, current_x, child_y);
        widget_set_size(child, child_width, child_height);

        current_x += child_width + data->spacing;
    }
}

static void update_vbox_layout(Widget *container)
{
    ContainerData *data = (ContainerData *)container->data;
    if (!data || data->child_count == 0)
        return;

    int visible_count = 0;
    for (int i = 0; i < data->child_count; i++)
    {
        if (data->children[i]->visible)
        {
            visible_count++;
        }
    }

    if (visible_count == 0)
        return;

    int available_width = container->width - (2 * data->padding);
    int available_height = container->height - (2 * data->padding) - ((visible_count - 1) * data->spacing);

    int total_fixed_height = 0;
    int flexible_count = 0;

    for (int i = 0; i < data->child_count; i++)
    {
        Widget *child = data->children[i];
        if (!child->visible)
            continue;

        if (child->height > 0)
        {
            total_fixed_height += child->height;
        }
        else
        {
            flexible_count++;
        }
    }

    int flexible_height = flexible_count > 0 ? (available_height - total_fixed_height) / flexible_count : 0;

    int total_content_height =
        total_fixed_height + (flexible_count * flexible_height) + ((visible_count - 1) * data->spacing);

    int current_y = container->y + data->padding;
    if (data->justify == ALIGN_CENTER && total_content_height < available_height)
    {
        current_y = container->y + (container->height - total_content_height) / 2;
    }
    else if (data->justify == ALIGN_END && total_content_height < available_height)
    {
        current_y = container->y + container->height - data->padding - total_content_height;
    }

    for (int i = 0; i < data->child_count; i++)
    {
        Widget *child = data->children[i];
        if (!child->visible)
            continue;

        int child_height = child->height > 0 ? child->height : flexible_height;
        int child_width;
        int child_x;

        if (data->alignment == ALIGN_STRETCH)
        {
            child_width = available_width;
            child_x = container->x + data->padding;
        }
        else
        {
            child_width = child->width > 0 ? child->width : available_width;

            if (data->alignment == ALIGN_CENTER)
            {
                child_x = container->x + data->padding + (available_width - child_width) / 2;
            }
            else if (data->alignment == ALIGN_END)
            {
                child_x = container->x + data->padding + available_width - child_width;
            }
            else // ALIGN_START
            {
                child_x = container->x + data->padding;
            }
        }

        widget_set_position(child, child_x, current_y);
        widget_set_size(child, child_width, child_height);

        current_y += child_height + data->spacing;
    }
}

static void update_grid_layout(Widget *container)
{
    ContainerData *data = (ContainerData *)container->data;
    if (!data || data->child_count == 0)
        return;

    int columns = data->grid_columns;
    int rows = (data->child_count + columns - 1) / columns;

    int available_width = container->width - (2 * data->padding) - ((columns - 1) * data->spacing);
    int available_height = container->height - (2 * data->padding) - ((rows - 1) * data->spacing);

    int cell_width = available_width / columns;
    int cell_height = available_height / rows;

    for (int i = 0; i < data->child_count; i++)
    {
        Widget *child = data->children[i];

        int row = i / columns;
        int col = i % columns;

        int child_x = container->x + data->padding + col * (cell_width + data->spacing);
        int child_y = container->y + data->padding + row * (cell_height + data->spacing);

        int child_width = cell_width;
        int child_height = cell_height;

        if (data->alignment == ALIGN_CENTER)
        {
            if (child->width > 0 && child->width < cell_width)
            {
                child_x += (cell_width - child->width) / 2;
                child_width = child->width;
            }
            if (child->height > 0 && child->height < cell_height)
            {
                child_y += (cell_height - child->height) / 2;
                child_height = child->height;
            }
        }
        else if (data->alignment != ALIGN_STRETCH)
        {
            if (child->width > 0)
                child_width = child->width;
            if (child->height > 0)
                child_height = child->height;
        }

        widget_set_position(child, child_x, child_y);
        widget_set_size(child, child_width, child_height);
    }
}

int container_get_child_count(Widget *container)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return 0;

    ContainerData *data = (ContainerData *)container->data;
    if (!data)
        return 0;

    return data->child_count;
}

Widget *container_get_child(Widget *container, int index)
{
    if (!container || container->type != WIDGET_TYPE_CONTAINER)
        return NULL;

    ContainerData *data = (ContainerData *)container->data;
    if (!data || index < 0 || index >= data->child_count)
        return NULL;

    return data->children[index];
}
