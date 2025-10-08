#!/usr/bin/env python3
import sys

def load_bdf(filename):
    with open(filename, 'r') as f:
        lines = [line.strip() for line in f]

    chars = {}
    i = 0

    while i < len(lines):
        parts = lines[i].split()
        if not parts:
            i += 1
            continue

        keyword = parts[0]

        if keyword == "STARTCHAR":
            char_name = parts[1] if len(parts) > 1 else ""
            char_data = {"name": char_name}
            i += 1

            while i < len(lines) and not lines[i].startswith("ENDCHAR"):
                char_parts = lines[i].split()
                if not char_parts:
                    i += 1
                    continue

                char_keyword = char_parts[0]

                if char_keyword == "ENCODING":
                    char_data["encoding"] = int(char_parts[1])
                elif char_keyword == "DWIDTH":
                    char_data["dwidth"] = int(char_parts[1])
                elif char_keyword == "BBX":
                    char_data["bbx"] = [int(x) for x in char_parts[1:5]]
                elif char_keyword == "BITMAP":
                    bitmap = []
                    i += 1
                    while i < len(lines) and not lines[i].startswith("ENDCHAR"):
                        if lines[i].strip():
                            bitmap.append(lines[i].strip())
                        i += 1
                    char_data["bitmap"] = bitmap
                    break

                i += 1

            if "encoding" in char_data:
                chars[char_data["encoding"]] = char_data

        i += 1

    return chars

def generate_c_header(chars):
    print("#ifndef BDF_FONT_H")
    print("#define BDF_FONT_H")
    print()
    print("typedef struct {")
    print("    int encoding;")
    print("    int width;")
    print("    int bbx_width;")
    print("    int bbx_height;")
    print("    int bbx_xoff;")
    print("    int bbx_yoff;")
    print("    const unsigned int* bitmap;")
    print("} bdf_char_t;")
    print()

    for encoding in sorted(chars.keys()):
        char_data = chars[encoding]
        bitmap = char_data.get("bitmap", [])
        bbx = char_data.get("bbx", [0, 0, 0, 0])

        print(f"static const unsigned int char_{encoding}_bitmap[] = {{")
        for hex_line in bitmap:
            value = int(hex_line, 16)
            print(f"    0x{value:02X},")
        print("};")
        print()

    print("static const bdf_char_t font_chars[] = {")
    for encoding in sorted(chars.keys()):
        char_data = chars[encoding]
        bbx = char_data.get("bbx", [0, 0, 0, 0])
        dwidth = char_data.get("dwidth", 0)
        print(f"    {{{encoding}, {dwidth}, {bbx[0]}, {bbx[1]}, {bbx[2]}, {bbx[3]}, char_{encoding}_bitmap}},")
    print("};")
    print()
    print(f"#define FONT_CHAR_COUNT {len(chars)}")
    print()
    print("#endif")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(1)

    chars = load_bdf(sys.argv[1])
    generate_c_header(chars)
