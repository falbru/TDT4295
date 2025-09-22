#include "widgets/grid.h"

Widget *grid_create(int x, int y, int width, int height, int columns)
{
    Widget *grid = container_create(x, y, width, height, LAYOUT_TYPE_GRID);
    if (grid)
    {
        container_set_grid_columns(grid, columns);
    }
    return grid;
}
