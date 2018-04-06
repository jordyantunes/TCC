import framediff
import matplotlib.pyplot as plt
import sys
import numpy as np
import util


def getdY(frame):
    return frame.dY


def main():
    args = np.array(sys.argv)
    args = args[1:]

    dY = np.vectorize(getdY)

    for i, v in enumerate(args):
        plt.figure(i)
        fingerprint = framediff.run(v)
        ass = dY(fingerprint)
        ass_lowpass = util.butter_lowpass_filter(ass, 3.5, 30.0)
        plt.title("Sinais pré e pós suavização")
        plt.plot(ass, label="Sinal original")
        plt.plot(ass_lowpass, label="Sinal filtrado")
        plt.legend()

    plt.show()


if __name__ == '__main__':
    main()
