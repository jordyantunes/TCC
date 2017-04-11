import sys
import numpy
import cv2
import math
# import file

def loadResource(source=0, out="outfile.avi"):
	# source serve para definir se a fonte sera um arquivo de video ou a webcam
	cap = cv2.VideoCapture(source)

	fourcc = cv2.VideoWriter_fourcc(*'DIVX')
	out = cv2.VideoWriter(out, fourcc, 20.0, (640,480))

	descritor_global = numpy.array([])

	saida = open("saida.txt", "w")

	while (cap.isOpened()):
		# ler frame
		ret, frame = cap.read()

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

		saida.write("\n-----------DESCRITOR--NORMAL---------\n")
		saida.write(str(descritor))

		descritor = numpy.sort(descritor, axis=-1, kind='quicksort')

		saida.write("\n-----------DESCRITOR--QUICADO---------\n")
		saida.write(str(descritor))

		descritor_global = numpy.append(descritor_global, descritor)

	return descritor_global

	cap.release()
	# out.release()
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