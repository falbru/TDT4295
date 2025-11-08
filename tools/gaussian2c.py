import numpy as np

def generate_gaussian_kernel(size=7, mean=None, std=1.0):
    """
    Generate a 2D Gaussian kernel.
    :param size: width/height of kernel (odd integer recommended)
    :param mean: optional mean offset (defaults to center)
    :param std: standard deviation
    :return: normalized kernel (values from 0–255, uint8)
    """
    if mean is None:
        mean = size // 2

    # Create a grid of (x,y) coordinates
    x, y = np.meshgrid(np.arange(size), np.arange(size))
    gaussian = np.exp(-((x - mean)**2 + (y - mean)**2) / (2 * std**2))

    # Normalize to 0–255 and convert to uint8
    gaussian = gaussian / np.max(gaussian) * 255.0
    return gaussian.astype(np.uint8)


def write_header(filename="brushstroke.h", kernel=None):
    """
    Write the Gaussian kernel to a C header file.
    """
    size = kernel.shape[0]
    with open(filename, "w") as f:
        f.write("#ifndef BRUSHSTROKE_H\n")
        f.write("#define BRUSHSTROKE_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"#define BRUSHSTROKE_WIDTH {size}\n")
        f.write(f"#define BRUSHSTROKE_HEIGHT {size}\n\n")
        f.write("static uint8_t brushstroke_diamond[] = {\n")

        for row in kernel:
            line = ", ".join(f"0x{val:02X}" for val in row)
            f.write(f"    {line},\n")

        f.write("};\n\n#endif\n")

    print(f"✅ Generated {filename} with {size}x{size} Gaussian kernel.")


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description="Generate a Gaussian kernel C header.")
    parser.add_argument("--size", type=int, default=7, help="Kernel size (odd integer)")
    parser.add_argument("--std", type=float, default=1.0, help="Standard deviation (σ)")
    parser.add_argument("--mean", type=float, default=None, help="Mean (center offset)")
    parser.add_argument("--output", type=str, default="brushstroke.h", help="Output header filename")

    args = parser.parse_args()

    kernel = generate_gaussian_kernel(size=args.size, mean=args.mean, std=args.std)
    write_header(args.output, kernel)
