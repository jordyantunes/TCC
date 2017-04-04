import numpy as np
import cv2
import sys

def frameDiff(video):
	descritores = np.array([[0, 0]])

	rec_1, frame_1 = video.read()

	while(video.isOpened()):
		rec_2, frame_2 = video.read()
		if rec_2 ==True:
			y = np.sum(frame_1)
			dY = np.sum(np.absolute(np.subtract(frame_1,frame_2)))
			rec_1, frame_1 = video.read()
			descritores = np.append(descritores, np.array([[dY, y]]), axis = 0)
			print(descritores)
		else:
			break
	
	return descritores 

def main():
	cap = cv2.VideoCapture(sys.argv[1])
	resultados = frameDiff(cap)
	print(resultados[0])

	cap.release()

if __name__ == "__main__":
	main()