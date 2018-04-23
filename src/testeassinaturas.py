from util import args, conn, comparadores, assinaturas
from comparador import Comparador
import numpy as np
import os


manhattan = comparadores[comparadores['nome'] == 'manhattan_distance'].iloc[0]

cursor = conn.cursor()
for i, needle in assinaturas.iterrows():
    needle_fingerprint = np.load(os.path.join(args.fingerprint_path, needle['filename'] + '.npy'))
    dY_needle = [n[0] for n in needle_fingerprint]
    for j, haystack in assinaturas.iterrows():
        haystack_fingerprint = np.load(os.path.join(args.fingerprint_path, haystack['filename'] + '.npy'))
        dY_haystack = [n[0] for n in haystack_fingerprint]
        diff = Comparador.manhattan_distance(dY_needle, dY_haystack)
        cursor.execute("""INSERT INTO testes
                       (id_video_original,
                       id_video_teste,
                       id_comparador,
                       data_criacao,
                       resultado_bruto)
                       VALUES
                       (?,?,?,time('now'),?)
                       """,
                       (needle['id'],
                        haystack['id'],
                        int(manhattan['id']),
                        diff
                        )
                       )
        conn.commit()

cursor.close()
conn.close()
