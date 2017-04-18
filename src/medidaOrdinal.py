import sys
import numpy
import cv2
import math
# import file

def loadResource(source=0, out=None):
	# source serve para definir se a fonte sera um arquivo de video ou a webcam

	print "--------------Medida ordinal---------------"
	cap = cv2.VideoCapture(source)

	descritor_global = numpy.array([])

	counter = 0

	if out is not None:
		saida = open(out, "w")

	while (cap.isOpened()):
		# ler frame
		ret, frame = cap.read()

		if counter % 24 == 0:
			print("Segundo {}".format(counter/24))
			
		counter += 1

		if numpy.shape(frame) == ():
			break

		# transforma em escala de cinza
		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		# muda o tamanho para 500,500 (pois o algoritmo pede resizing)
		gray = cv2.resize(gray, (500,500), fx=0, fy=0)



		descritor = numpy.array([0.0])

		for xquadro in range(5):
			for yquadro in range(5):
				acumulador = 0
				for x in range(10):
					for y in range(10):
						acumulador += gray[xquadro*100 + x, yquadro*100 + y]
				descritor = numpy.append(descritor, acumulador)	

		descritor = numpy.sort(descritor, axis=-1, kind='quicksort')


		if out is not None:
			for d in descritor:
				saida.write(str(d) + "\n")

		descritor_global = numpy.append(descritor_global, descritor)

	return descritor_global

	cap.release()

	if out is not None:
		saida.release()
	
	cv2.destroyAllWindows()

def getGradient(frame, x=0, y=0):
	return cv2.Sobel(frame, cv2.CV_64F, x, y, ksize=3)

def main():
	v1 = loadResource(sys.argv[1])
	# v2 = loadResource(sys.argv[1])

	print v1

	# for x in range(v1.shape[0]):
	# 	print v1[x] - v2[x]

if __name__ == '__main__':
	main()