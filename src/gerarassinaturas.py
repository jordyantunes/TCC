import argparse
import sqlite3
# from .comparador import Comparador
import trueframediff
import wavelet
import rbp
import medidaordinal
import gradiente
import sceneframe
import os
import logging
import sys
import pandas as pd
import re
import numpy as np
from util import args, conn, videos, algoritmos


# ------- constants
extensoes = ['.mp4', '.avi']

geradores = {'trueframediff': trueframediff.framediff.run,
             'wavelets': wavelet.wavelet.run,
             'gradiente': gradiente.gradiente.run,
             'sceneframe': sceneframe.sceneframe.run,
             'medidaordinal': medidaordinal.medidaordinal.run,
             'rbp': rbp.rbp.run}

extensions_regex = re.compile("|".join(extensoes))

videos = videos[np.isnan(videos['video_original'])]

for i, v in videos.iterrows():
    sem_extensao = extensions_regex.sub("", v['filename'])

    # framediff
    video_path = os.path.join(args.video_path, v['filename'])

    logging.info("Video {}, framediff".format(video_path))

    if not os.path.isfile(video_path):
        print("Not a file")
        continue

    for index, alg in geradores.items():
        assinatura = alg(video_path)
        assinatura_filename = sem_extensao + "_" + index
        algoritmo = algoritmos[algoritmos['slug'] == 'framediff'].iloc[0]['id']

        np.save(os.path.join(args.fingerprint_path,
                             assinatura_filename), assinatura)

        cursor = conn.cursor()
        cursor.execute("INSERT INTO assinaturas (id_video, id_algoritmo, filename) VALUES (?,?,?)",
                       (v['id'], int(algoritmo), assinatura_filename)
                       )
        conn.commit()
        cursor.close()
