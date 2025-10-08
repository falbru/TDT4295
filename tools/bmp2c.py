#!/usr/bin/env python3

import sys
from PIL import Image

def bitmap_to_c_header(image_path):
    img = Image.open(image_path).convert('L')
    width, height = img.size
    pixels = list(img.getdata())

    header_name = image_path.split('/')[-1].split('.')[0].upper()

    print(f"#ifndef {header_name}_H")
    print(f"#define {header_name}_H")
    print()
    print("#include <stdint.h>")
    print()
    print(f"#define {header_name}_WIDTH {width}")
    print(f"#define {header_name}_HEIGHT {height}")
    print()
    print(f"const uint8_t {header_name.lower()}_data[] = {{")

    for i, pixel in enumerate(pixels):
        if i % 16 == 0:
            print("    ", end="")
        print(f"0x{pixel:02X}", end="")
        if i < len(pixels) - 1:
            print(", ", end="")
        if (i + 1) % 16 == 0:
            print()

    if len(pixels) % 16 != 0:
        print()

    print("};")
    print()
    print(f"#endif // {header_name}_H")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)
    bitmap_to_c_header(sys.argv[1])
