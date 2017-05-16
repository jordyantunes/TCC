import sys
import numpy
import cv2
import math

def loadResource(source=0, out=None):
	# source serve para definir se a fonte sera um arquivo de video ou a webcam

	print "--------------Gradiente---------------"

	cap = cv2.VideoCapture(source)

	descritor_global = numpy.array([])

	counter = 0

	if out is not None:
		saida = open(out, "w")

	while (cap.isOpened()):

		if counter % 24 == 0:
			print("Segundo {}".format(counter/24))

		counter += 1

		# ler frame
		ret, frame = cap.read()

		if numpy.shape(frame) == ():
			break

		# transforma em escala de cinza
		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		# muda o tamanho para 500,500 (pois o algoritmo pede resizing)
		gray = cv2.resize(gray, (320,240), fx=0, fy=0)
		tamx = 320 / 4
		tamy = 240 / 3

		gradientex = getGradient(gray, 1, 0)
		gradientey = getGradient(gray, 0, 1)

		shape = gradientex.shape

		orientacao = numpy.array([ [ math.atan2(gradientex[x,y],gradientey[x,y]) for y in range(shape[1]) ] for x in range(shape[0]) ])
		magnitude = numpy.array([ [ math.sqrt(math.pow(gradientex[x,y],2) + math.pow(gradientey[x,y], 2)) for y in range(shape[1]) ] for x in range(shape[0]) ])

		# cv2.imshow("gradientex", gradientex)
		# cv2.imshow("gradientey", gradientey)

		descritor = numpy.array([0.0])

		for xquadro in range(3):
			for yquadro in range(4):
				dividendo = 0
				denominador = 0
				for x in range(tamx):
					for y in range(tamy):
						dividendo += magnitude[(xquadro * tamx) + x, (yquadro * tamy) + y] * orientacao[(xquadro * tamx) + x, (yquadro * tamy) + y]
						denominador += magnitude[(xquadro * tamx) + x, (yquadro * tamy) + y]
				try:
					descritor = numpy.append(descritor, dividendo/denominador)
				except:
					print(dividendo)
					print(denominador)

		if out is not None:
			saida.write(str(descritor))

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

	for x in range(v1.shape[0]):
		print v1[x] - v2[x]

if __name__ == '__main__':
	main()