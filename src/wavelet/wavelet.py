import pywt
import cv2
import numpy as np
import sys

img = cv2.imread(sys.argv[1], 0)

haar = pywt.wavedec2(img, 'haar', level=3)

for i in range(len(haar)):
	if i == 0:
		cv2.imshow(("cA %d"% i), haar[i])
	else:
		cv2.imshow("cH" + str(i), haar[i][0])
		cv2.imshow("cV" + str(i), haar[i][1])
		cv2.imshow("cD" + str(i), haar[i][2])

# cv2.imshow("cA", haar[0])
# cv2.imshow("cH", haar[1])
# cv2.imshow("cV", haar[2])
# cv2.imshow("cD", haar[3])

cv2.waitKey(0)