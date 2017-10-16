import pywt
import cv2
import numpy as np
import sys
import math

def main():
    # constants
    iterations = 5
    partitions_num = 9
    x_partitions = y_partitions = int(math.floor(math.sqrt(partitions_num)))

    # read image
    img = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)
    out = np.zeros(img.shape)

    # normalize image
    normalized = np.float64(img) / 255

    haar = pywt.wavedec2(normalized, 'haar', level=iterations)

    for w in range(iterations + 1):
        print(w)
        if w == 0:
            sh = haar[w].shape
            print("shape destino {}".format(out[0:sh[0],0:sh[1]].shape))
            out[0:sh[0],0:sh[1]] = haar[w]
        else:
            cH, cV, cD = haar[w]
            sh = cH.shape
            # cH
            print("shape destino {}".format(out[(sh[0]):(sh[0]*2), 0:(sh[1])].shape))
            out_shape = out[(sh[0]):(sh[0]*2), 0:(sh[1])].shape
            out[(sh[0]):(sh[0]*2), 0:(sh[1])] = cH[:out_shape[0],:out_shape[1]]
            
            # cV
            sh = cV.shape
            print("shape destino {}".format(out[0:(sh[0]), (sh[1]):(sh[1]*2)].shape))
            out_shape = out[0:(sh[0]), (sh[1]):(sh[1]*2)].shape
            out[0:(sh[0]), (sh[1]):(sh[1]*2)] = cV[:out_shape[0],:out_shape[1]]

            # cD
            sh = cD.shape
            print("shape destino {}".format(out[(sh[0]):(sh[0]*2), (sh[1]):(sh[1]*2)].shape))
            out_shape = out[(sh[0]):(sh[0]*2), (sh[1]):(sh[1]*2)].shape
            out[(sh[0]):(sh[0]*2), (sh[1]):(sh[1]*2)] = cD[:out_shape[0],:out_shape[1]]

    # out = ((out - 0.5) * 2) + 0.5
    cv2.imshow("Test", out)
    nome = sys.argv[1].split(".")
    nome = nome[0] + "_haar_." + nome[1]
sww22    print("Saving to... {}".format(nome))
    cv2.imwrite(nome, out * 255)
    cv2.waitKey(0)


if __name__ == '__main__':
    main()