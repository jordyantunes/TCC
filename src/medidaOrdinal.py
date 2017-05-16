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
	
	while (cap.isOpened()):
		# ler frame
		ret, frame = cap.read()

		if counter % 24 == 0:
			print("Segundo {}".format(counter/24))
			
		counter += 1

		if numpy.shape(frame) == ():
			break

		# transforma em escala de cinza
		gray = pre_process(frame, 500, 500)

		descritor = frame_to_fingerprint(gray, 500,500,5,5)

		descritor_global = numpy.append(descritor_global, descritor)

	return descritor_global

	cap.release()

	if out is not None:
		saida.release()
	
	cv2.destroyAllWindows()

def pre_process(frame, x, y):
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# muda o tamanho para 500,500 (pois o algoritmo pede resizing)
	gray = cv2.resize(gray, (500,500), fx=0, fy=0)

	return gray

def frame_to_fingerprint(frame, x, y, divx, divy):
	descritor = numpy.array([0.0])

	for xquadro in range(divx):
		for yquadro in range(divy):
			acumulador = 0
			for x in range(x/divx):
				for y in range(y/divy):
					acumulador += frame[xquadro*100 + x, yquadro*100 + y]
			descritor = numpy.append(descritor, acumulador)	

	descritor = numpy.sort(descritor, axis=-1, kind='quicksort')

	return descritor

def main():
	v1 = loadResource(sys.argv[1])
	v2 = loadResource(sys.argv[1])

	for x in range(v1.shape[0]):
		if v1[x] != v2[x]:
			print v1[x] - v2[x]

if __name__ == '__main__':
	main()