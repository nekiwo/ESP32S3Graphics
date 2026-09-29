# Texture file layout:
# - Header
#   - uint32_t width
#   - uint32_t height
#   - uint32_t length
# - uint8_t data[]

from PIL import Image
import sys

if len(sys.argv) < 3:
    print("Not enough arguments.")
    exit()

original = Image.open(sys.argv[1])
original = original.convert("RGB")

width, height = original.size

encoded = open(sys.argv[2], "wb")

encoded.write(width.to_bytes(4, byteorder="little"))
encoded.write(height.to_bytes(4, byteorder="little"))

data_array = []
for x in range(width):
    for y in range(height):
        pixel = original.getpixel((y, x))
        red = int(round(pixel[0] / 255.0))
        green = int(round(pixel[1] / 255.0))
        blue = int(round(pixel[2] / 255.0))
        
        new_pixel = (red << 7) | (green << 6) | (blue << 5)
        data_array.append(new_pixel.to_bytes(1))
        continue

encoded.write(len(data_array).to_bytes(4, byteorder="little"))

for byte in data_array:
    encoded.write(byte)

encoded.close()