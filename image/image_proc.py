import cv2
import numpy
import matplotlib.pyplot as plt

image_path = "./numbers.png"
image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
image = cv2.blur(image, (2 ,2), 0)

image = (image - image.min()) / image.max() # * 2 - 1
weight = 100

for row in image:
    print("{", end='')
    for i, ele in enumerate(row):
        print(ele, end='')
        if i < weight - 1:
            print(", ", end='')
    print("},")

plt.imshow(image)
plt.show()

