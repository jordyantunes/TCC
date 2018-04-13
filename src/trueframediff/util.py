import numpy as np
from scipy.signal import butter, lfilter, freqz


def butter_lowpass(cutoff, fs, order=5):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return b, a


def butter_lowpass_filter(data, cutoff, fs, order=5):
    b, a = butter_lowpass(cutoff, fs, order=order)
    y = lfilter(b, a, data)
    return y


def getParam(vect, param):
    return getattr(vect, param)


def findPeaks(feature, n, threshold):
    out = (np.array([]), np.array([]))
    indices_validos = np.array([])
    picos = np.array([])

    for i in range(0, feature.shape[0]):
        current_peak = feature[i]
        candidates = feature[i:]
        indices_validos = np.where(candidates > current_peak * threshold)[0]
        picos = candidates[indices_validos]

        indices_validos = np.add(indices_validos, i)

        # print("Curr {}, thres {}, candidates {}".format(current_peak, current_peak * threshold, indices_validos))

        if (indices_validos.shape[0] >= n):
            break

    print("Indices {}, n {}".format(indices_validos.shape[0], n))
    if (indices_validos.shape[0] < n):
            raise Exception("Número de indices({}) < n({})".format(picos.shape[0], n))

    return (indices_validos, picos)


getParamVectorized = np.vectorize(getParam)
