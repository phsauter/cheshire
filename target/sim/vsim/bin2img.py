import sys
from PIL import Image
image = Image.new("RGB", (800, 600), 0);
pixels = image.load()
print("Opening file " + sys.argv[1])
content = 0
with open(sys.argv[1], "rb") as file:
    content = file.read()

print(type(content))
x = 0
y = 0

for y in range(0, 600):
    for x in range(0, 800):
        idx = 4 * (y * 800 + x)
        pixels[x, y] = (content[idx], content[idx + 1], content[idx + 2])
image.save(sys.argv[1] + ".png")