import sys
import numpy
import cv2
import math


def loadResource(source, out=None):
    # source serve para definir se a fonte sera um arquivo de video ou a webcam

    print("--------------Gradiente---------------")

    cap = cv2.VideoCapture(source)

    descritor_global = None

    counter = 0

    # if out is not None:
    #     saida = open(out, "w")

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
        tamx = int(320 / 4)
        tamy = int(240 / 3)

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

        if descritor_global is None:
            descritor_global = numpy.array([descritor])
        else:
            descritor_global = numpy.append(descritor_global, [descritor], axis=0)

    return descritor_global

    cap.release()
    # if out is not None:
    #     saida.release()
    # cv2.destroyAllWindows()


def getGradient(frame, x=0, y=0):
    return cv2.Sobel(frame, cv2.CV_64F, x, y, ksize=3)


def run(video_path, filename=None):
    resultados = loadResource(video_path)

    if filename is not None:
        numpy.savetxt("{}_gradiente.csv".format(filename), resultados, delimiter=",")

    return resultados


def main():
    run(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
