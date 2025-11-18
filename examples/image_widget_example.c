#include "widgets/image_widget.h"
#include "primitives/image.h"
#include "color.h"

static const Color sample_image_data[] = {
    COLOR_RGB(255, 0, 0), COLOR_RGB(0, 255, 0), COLOR_RGB(0, 0, 255),
    COLOR_RGB(255, 255, 0), COLOR_RGB(255, 0, 255), COLOR_RGB(0, 255, 255),
    COLOR_RGB(128, 128, 128), COLOR_RGB(255, 255, 255), COLOR_RGB(0, 0, 0)
};

int main() {
    Image sample_image = {
        .data = sample_image_data,
        .width = 3,
        .height = 3
    };

    Widget *img = image_widget_create(10, 10, &sample_image);
    image_widget_set_border(img, COLOR_BLACK, 2);

    return 0;
}
