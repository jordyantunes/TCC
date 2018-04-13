import numpy as np


class Comparador:
    def __init__(self, video_original, video_teste):
        self.video_original = video_original
        self.video_teste = video_teste

    def comparar():
        raise NotImplementedError()


class ComparadorComum (Comparador):
    def __init__(self, video_original, video_teste):
        super().__init__(video_original, video_teste)

    def comparar(self):
        n_quadros = self.video_original.shape[0]
        k_items = self.video_original[0].shape[0]

        print("{} quadros e {} itens".format(n_quadros, k_items))

    def __L(f1, f2):
        return np.sum(f1 - f2)


class ComparadorManhattan (Comparador):
    def __init__(self, video_original, video_teste):
        super().__init__(video_original, video_teste)

    def comparar(self):
        n_quadros = self.video_original.shape[0]
        k_items = self.video_original[0].shape[0]

        print("{} quadros e {} itens".format(n_quadros, k_items))

    def __L(self):
        return np.sum(np.abs(self.video_original - self.video_teste)) / self.video_original.shape[0]
