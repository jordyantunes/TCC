import numpy as np
from comparador import Comparador
import cv2
import algoritmos
import sys


def main():

    print("Carregando video original")
    v_original = cv2.VideoCapture(sys.argv[1])

    print("Carregando video teste")
    v_teste = cv2.VideoCapture(sys.argv[2])

    # frameDiff
    # original = algoritmos.frameDiff.frameDiff(v_original)
    # teste = algoritmos.frameDiff.frameDiff(v_teste)

    # gradiente
    original = algoritmos.medidaOrdinal.ordinalMeasure(sys.argv[1])
    teste = algoritmos.medidaOrdinal.ordinalMeasure(sys.argv[2])

    comp = Comparador.ComparadorComum(original, teste)
    diff = comp.comparar()

    print(diff)


if __name__ == '__main__':
    main()
