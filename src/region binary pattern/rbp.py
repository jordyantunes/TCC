import numpy as np
import cv2
import random
import sys
import math


def dec2bin(x, p):
    return [math.floor(x / (2^(p_ - 1))) % 2 for p_ in range(p,1,-1)]


def bin2dec(b, p):
    acumulador = 0
    
    for p_ in range(p):
        acumulador = acumulador + (b[0] * (2 ^ p_))


def cbin(b, r):
    P = len(b)
    return [b[(p-1 + r)%P] for p in range(P, 1, -1)]


def get_color(position, length, min=0, max=255):
    return tuple([random.randint(min, max) if c == position else 0 for c in range(length)])


def generate_mask(width, height, rings=3, subregions=6):
    imagem = np.zeros((width, height, 1), np.uint8)    

    rings = 5
    subregions = 6

    total_radius = width / 2
    c_radius = [(total_radius / rings) * (c + 1) for c in reversed(range(rings))]
    c_angles = [(360 / subregions) * i for i in range(subregions + 1)]

    region_count = 0

    for indice_raio, raio in enumerate(c_radius):
        for indice_angulo, angulo in enumerate(c_angles[:6]):
            region_count = cor = region_count + 1
            cv2.ellipse(imagem,
                (int(width / 2), int(height / 2)),
                (int(raio), int(raio)),
                0,
                int(angulo),
                int(c_angles[(indice_angulo + 1) % len(c_angles)]),
                cor,
                -1)

    return (imagem, rings, subregions)


def mean_luminance(img, mask, rings, subregions):
    (width, height, channels) = np.shape(mask)

    # TODO COMPARAR SHAPE

    counter = {}
    sum = {}

    for w in range(width):
        for h in range(height):
            im = mask[w, h][0]
            if im != 0:
                counter[im] = counter[im] + 1 if im in counter else 1
                sum[im] = sum[im] + img[w, h] if im in sum else img[w, h]

    sub_luminancia = np.zeros((rings, subregions), np.float32)
    luminancia_total = 0.0

    for subr in counter:
        ring = int(( subr - 1 ) / subregions)
        subregion = int(subr % subregions)

        sub_luminancia[ring, subregion] = sum[subr] / float(counter[subr])

    counter_values = [v for v in counter.items()]

    luminancia = np.sum([v for v in sum.items()]) / float(np.sum(counter_values))

    return (sub_luminancia, luminancia)


def generate_rbp(sub_luminancia, luminancia):
    b_np = [[1 if subregion >= luminancia else 0 for subregion in subregions]
        for subregions in sub_luminancia
   ]

    inter = [[1 if sub_luminancia[indice_anel, indice_sub] >= sub_luminancia[indice_anel + 1, indice_sub] else 0 for indice_sub in range(len(sub_luminancia[0]))] 
        for indice_anel in range(len(sub_luminancia) - 1)
   ]
    
    return b_np + inter


def rbp(source):
    print("--------------Wavelets---------------")
    cap = cv2.VideoCapture(source)

    descritor_final = np.array([])

    length = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    counter = 0

    while (cap.isOpened()):
        counter += 1
        print("\r{}/{}({:.2f}%)".format(counter, length, (counter/length)*100),end='')
        # ler frame
        ret, frame = cap.read()

        if np.shape(frame) == ():
            break
        

        imagem = frame
        imagem = cv2.cvtColor(imagem, cv2.COLOR_BGR2GRAY)
        imagem = cv2.resize(imagem, (500, 500), fx=0, fy=0)
        
        # cv2.imshow("Frame", imagem)

        (mask, rings, subregions) = generate_mask(500, 500)

        sub_luminancia, luminancia = mean_luminance(imagem, mask, rings, subregions)

        rbp = generate_rbp(sub_luminancia, luminancia)
        descritor_final = np.append(descritor_final, [rbp])
    
        # cv2.waitKey(1)

    return descritor_final


def run(video_path, filename):
    resultados = rbp(video_path)
    print(resultados[:10])
    np.savetxt("{}_rbp.csv".format(filename), resultados, delimiter=",")


def main():
    run(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
