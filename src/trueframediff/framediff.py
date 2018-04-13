import cv2
import sys
import numpy as np
from math import log, pow
import util
import peakutils


def getvideoindex(feat, w, n, t):
    """
        Controi o indice da assinatura

        :param feat: <Features> feature
        :param w: Tamanho da janela
        :param n: Número de índices
        :param t: Limiar para encontrar picos
        :return Tupla contendo indices dos picos e os valores
        :rtype (np.array, np.array)
    """
    dY = util.getParamVectorized(feat, 'dY')
    dY = util.butter_lowpass_filter(dY, 3.5, 30.0)
    stillness = util.getParamVectorized(feat, 'stillness')
    credits = util.getParamVectorized(feat, 'credits')

    feat_filter = np.logical_and(stillness < 90, credits < 90)
    dY = dY[feat_filter][:w]
    peak_indices = peakutils.indexes(dY, thres=t, min_dist=15)
    # peak_indices, peaks = util.findPeaks(dY, n, t)

    peak_indices = peak_indices.astype(int)
    indices = dY[peak_indices]

    if indices.shape[0] < n:
        raise Exception("Número de picos({}) < n({})".format(indices.shape[0], n))

    indices = indices[:n]
    peak_indices = peak_indices[:n]

    return (peak_indices, indices)


class Features:
    def __init__(self, frame, t, Y, Ymax, A, dY):
        self.frame = frame
        self.t = t,
        self.Y = Y
        self.Ymax = Ymax
        self.A = A
        self.dY = dY

        try:
            self.stillness = 100 * (1 - np.sqrt(log(dY / A) / log(256))) if dY is not None and dY > 0 else 0.0
            self.credits = 100 * (((Ymax / 256) + pow(1 - (log(Y / A) / log(256)),2)) / 2) if Y > 0 else 0.0
        except Exception as e:
            print(e)
            print("dY: {} | Y: {} | A: {}".format(dY, Y, A))
            exit(1)


def preprocessamento(frame):
    return cv2.resize(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), (176, 144))


def framediff(source):
    print("--------------True FrameDiff---------------")
    cap = cv2.VideoCapture(source)
    out = None

    frame_array = np.array([])
    length = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    cnt = 0

    while cap.isOpened():
        frame_exists, frame = cap.read()

        if not frame_exists:
            break

        print("\r{}/{} ({:.1f}%)".format(cnt, length, cnt / length * 100), end="")
        cnt += 1

        imagem = preprocessamento(frame)

        features = Features(frame,
                            cap.get(cv2.CAP_PROP_POS_MSEC),  # current timestamp
                            np.sum(imagem),  # total luminance
                            np.max(imagem),  # max luminance
                            frame.shape[0] * frame.shape[1],  #frame area
                            np.sum(frame - frame_array[frame_array.shape[0] - 1].frame) if frame_array.shape[0] > 0 else np.sum(frame)) # luminance diff

        frame_array = np.append(frame_array, features)

        # cv2.imshow("Trailer", imagem)
        # if cv2.waitKey(1) & 0xFF == ord('q'):
        #     break
    print("")
    return frame_array


def run(source):
    return framediff(source)


def main():
    run(sys.argv[1])
    # run(sys.argv[1], sys.argv[2])
    # run()


if __name__ == '__main__':
    main()
