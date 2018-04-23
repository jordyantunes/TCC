import sys
import numpy
import cv2
import math
from itertools import product
# import file


def ordinalMeasure(source):
    # source serve para definir se a fonte sera um arquivo de video ou a webcam

    print("--------------Medida ordinal---------------")
    cap = cv2.VideoCapture(source)
    descritor_global = None

    counter = 0
    
    while (cap.isOpened()):
        # ler frame
        ret, frame = cap.read()

        if numpy.shape(frame) == ():
            break

        # transforma em escala de cinza
        gray = pre_process(frame, 500, 500)

        descritor = frame_to_fingerprint(gray, 5, 5)

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


def frame_to_fingerprint(frame, divx, divy):
    descritor = numpy.array([])

    for x, y in product(range(divx), range(divy)):
        descritor = numpy.append(descritor, numpy.average(frame[100*x:100*x+100, 100*y:100*y+100]))    

    descritor = numpy.sort(descritor, axis=-1, kind='quicksort')

    return descritor


def run(video_path, filename=None):
    resultados = ordinalMeasure(video_path)

    if filename is not None:
    	numpy.savetxt("{}_ordinal.csv".format(filename), resultados, delimiter=",")

    return resultados


def main():
    run(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
    main()