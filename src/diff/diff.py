import numpy as np
import cv2
import sys

f1 = sys.argv[1]
f2 = sys.argv[2]

img1 = cv2.imread(f1, cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread(f2, cv2.IMREAD_GRAYSCALE)

diff = img1 - img2

cv2.imwrite('diff.png', diff)