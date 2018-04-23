import numpy as np
from scipy import spatial
import fastdtw


def dtw(a, b):
    a = np.array(a)
    b = np.array(b)
    length = a.shape[0] if a.shape[0] < b.shape[0] else b.shape[0]
    distance, path = fastdtw(a[:length], b[:length], dist=spatial.distance.euclidean)
    return distance


def manhattan_distance(a, b):
    a = np.array(a)
    b = np.array(b)
    length = a.shape[0] if a.shape[0] < b.shape[0] else b.shape[0]
    return np.sum(np.abs(a[:length] - b[:length])) / length


def euclidean_distance(a, b):
    a = np.array(a)
    b = np.array(b)
    length = a.shape[0] if a.shape[0] < b.shape[0] else b.shape[0]
    return spatial.distance.euclidean(a[:length], b[:length])


# class Comparador:
#     def __init__(self, video_original, video_teste):
#         self.video_original = video_original
#         self.video_teste = video_teste

#     def comparar():
#         raise NotImplementedError()


# class ComparadorComum (Comparador):
#     def __init__(self, video_original, video_teste):
#         super().__init__(video_original, video_teste)

#     def comparar(self):
#         n_quadros = self.video_original.shape[0]
#         k_items = self.video_original[0].shape[0]

#         print("{} quadros e {} itens".format(n_quadros, k_items))

#     def __L(f1, f2):
#         return np.sum(f1 - f2)


# class ComparadorManhattan (Comparador):
#     def __init__(self, video_original, video_teste):
#         super().__init__(video_original, video_teste)

#     def comparar(self):
#         n_quadros = self.video_original.shape[0]
#         k_items = self.video_original[0].shape[0]

#         print("{} quadros e {} itens".format(n_quadros, k_items))

#     def __L(self):
#         return np.sum(np.abs(self.video_original - self.video_teste)) / self.video_original.shape[0]
