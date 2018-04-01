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
        n_quadros = None
        k_items = None

        if self.video_original.shape[0] > self.video_teste.shape[0]:
            print("Original maior")
            n_quadros = self.video_teste.shape[0]
            k_items = self.video_original[0].shape[0]
            self.video_original = self.video_original[:n_quadros]
        else:
            print("Original menor ou igual")
            n_quadros = self.video_original.shape[0]
            k_items = self.video_original[0].shape[0]
            self.video_teste = self.video_teste[:n_quadros]

        print("{} quadros e {} itens".format(n_quadros, k_items))

        return np.sum(
            np.abs(self.video_original - self.video_teste)
            ) / (n_quadros * k_items)

