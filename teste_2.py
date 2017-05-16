import sys
import numpy
import cv2

def main():
	cap = cv2.VideoCapture(0)

	while (cap.isOpened()):
		ret, frame = cap.read()

		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		# gray = cv2.flip(gray, 1)
		# cv2.putText(gray, "Mike Wazowski", (100,300), font, 2, (255,255,255))

		cv2.imshow('original', gray)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main()