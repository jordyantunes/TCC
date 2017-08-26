import numpy as np
import cv2
import random
import argparse


def get_color(position, length, min=0, max=255):
	return tuple([ random.randint(min, max) if c == position else 0 for c in range(length)])
	
width  = 500
height = 500

n_circles = 3
n_subregions = 6

total_radius = width/2
c_radius = [ (total_radius/n_circles) * (c+1) for c in reversed(range(n_circles))]
c_angles = [ (360 / n_subregions) * i for i in range(n_subregions + 1)]

imagem = np.zeros((500,500, 3), np.uint8)
random.seed()

for indice_raio, raio in enumerate(c_radius):
	for indice_angulo, angulo in enumerate(c_angles[:6]):
		cor = get_color(indice_raio, 3)
		cv2.ellipse(imagem, (250,250), (raio,raio), 0, angulo, c_angles[(indice_angulo + 1)%len(c_angles)], cor, -1)
# for index, c in enumerate(c_radius):
# 	cv2.circle(imagem, (250,250), c, color[index], -1)

cv2.imshow("circulo", imagem)

cv2.waitKey(0)