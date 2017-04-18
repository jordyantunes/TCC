import sys
import numpy
import cv2

def main():
	# img = cv2.imread(sys.argv[1], 0)
	# cv2.imshow('katya', img)
	# cv2.imwrite('katya-bw.png', img)
	cap = cv2.VideoCapture(0)
	font = cv2.FONT_HERSHEY_SIMPLEX

	fourcc = cv2.VideoWriter_fourcc(*'DIVX')
	out = cv2.VideoWriter('mikewazowski.avi', fourcc, 20.0, (640,480))

	while (cap.isOpened()):
		ret, frame = cap.read()

		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		# gray = cv2.flip(gray, 1)
		# cv2.putText(gray, "Mike Wazowski", (100,300), font, 2, (255,255,255))

		cv2.imshow('original', gray)

		out.write(cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR))
		sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0)
		sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1)

		cv2.imshow("sobelx", sobelx)
		cv2.imshow("sobely", sobely)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	cap.release()
	# out.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main()