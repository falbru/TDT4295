#include "widgets/vbox.h"

Widget *vbox_create(int x, int y, int width, int height)
{
    return container_create(x, y, width, height, LAYOUT_TYPE_VBOX);
}
