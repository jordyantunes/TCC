import sys
import numpy
import cv2
import math
# import file

def ordinalMeasure(source):
	# source serve para definir se a fonte sera um arquivo de video ou a webcam

	print "--------------Medida ordinal---------------"
	cap = cv2.VideoCapture(source)
	descritor_global = None

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

		if descritor_global is None:
			descritor_global = numpy.array([descritor])
		else:
			descritor_global = numpy.append(descritor_global, [descritor], axis=0)

	cap.release()

	return descritor_global

def pre_process(frame, x, y):
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# muda o tamanho para 500,500 (pois o algoritmo pede resizing)
	gray = cv2.resize(gray, (500,500), fx=0, fy=0)

	return gray

def frame_to_fingerprint(frame, width, heigth, divx, divy):
	descritor = numpy.array([])

	for xquadro in range(divx):
		for yquadro in range(divy):
			acumulador = 0
			for x in range(width/divx):
				for y in range(heigth/divy):
					acumulador += frame[xquadro*100 + x, yquadro*100 + y]
			descritor = numpy.append(descritor, acumulador)	

	descritor = numpy.sort(descritor, axis=-1, kind='quicksort')

	return descritor

def run(video_path, filename):
	resultados = ordinalMeasure(video_path)
	numpy.savetxt("{}_ordinal.csv".format(filename), resultados, delimiter=",")

def main():
	run(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
	main()