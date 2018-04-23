import pywt
import cv2
import numpy as np
import sys
import math


def waveletDescriptor(source):
    print("--------------Wavelets---------------")
    cap = cv2.VideoCapture(source)
    out = None

    counter = 0

    descritor_final = np.array([])

    while (cap.isOpened()):
        # ler frame
        ret, frame = cap.read()

        if np.shape(frame) == ():
            break

        # constants
        iterations = 5
        partitions_num = 9
        x_partitions = y_partitions = int(
            math.floor(math.sqrt(partitions_num)))

        # read image
        img = frame
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        out = np.zeros(img.shape)

        # normalize image
        normalized = np.float64(img) / 255

        haar = pywt.wavedec2(img, 'haar', level=iterations)

        # wavelet-based signature
        w_descriptor = np.zeros((iterations + 1, 3))
        for w in reversed(range(iterations + 1)):
            position = iterations - w
            if w > 0:
                cH, cV, cD = haar[w]
                w_descriptor[position,:] = get_energy(cH), get_energy(cV), get_energy(cD)
            else:
                w_descriptor[position,0] = get_energy(haar[w])

        # spatial distribution of pixel gradient
        cA, (cH, cV, cD) = pywt.dwt2(normalized, 'haar') 
        x, y = cv2.Sobel(cA, cv2.CV_64F, 1, 0), cv2.Sobel(cA, cv2.CV_64F, 0, 1)
        mag = cv2.magnitude(x,y)

        x_pixels, y_pixels = np.shape(mag)
        x_pixels, y_pixels = int(math.floor(x_pixels / x_partitions)) , int(math.floor(y_pixels / y_partitions))

        gradient_signature = np.array([None for x in range(partitions_num)])

        for x in range(x_partitions):
            for y in range(y_partitions):
                part = mag[x * x_pixels:x * x_pixels + x_pixels, y * y_pixels: y * y_pixels + y_pixels]
                # cv2.imshow("L{}{}".format(x,y), part)
                gradient_signature[y_partitions * x + y] = np.histogram(part)

        descritor_final = np.append(descritor_final, [w_descriptor, gradient_signature])
        # cv2.waitKey(0)
    
    return descritor_final


def get_energy(input):
    shape = np.shape(input)
    e = np.sum(np.absolute(input)) / (shape[0] * shape[1])
    return e


def run(video_path, filename=None):
    resultados = waveletDescriptor(video_path)
    # nome_arquivo_saida = "{}_wavelet.csv".format(filename)

    if filename is not None:
        np.save(filename, resultados)

    return resultados


def main():
    run(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
