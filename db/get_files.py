import os
import sqlite3
import argparse
import logging
import sys
import re
import pandas as pd


def get_video_pai(sem_extensao):
    global videos_ids
    global distorcoes_db
    global extensoes

    video_pai = None

    distorcoes_strings = [d + "__" for d in distorcoes_db['slug'].values.tolist()]
    regex_pattern = "|".join(distorcoes_strings)
    distorcoes_regex = re.compile(regex_pattern, re.IGNORECASE)
    video_pai = distorcoes_regex.sub("", sem_extensao) 
    # pais_possiveis = [video_pai + e for e in extensoes]
    pais_possiveis = [video_pai]

    id_video_pai = videos_db[videos_db['filename'].isin(pais_possiveis)]
    id_video_pai = id_video_pai.iloc[0]['id'] if id_video_pai.shape[0] > 0 else None


    if id_video_pai is None and video_pai in videos_ids:
        id_video_pai = videos_ids[video_pai]

    # indice_distorcao = sem_extensao.rfind("_")
    # video_pai = sem_extensao[:indice_distorcao if indice_distorcao > 0 else len(sem_extensao)]
    # pais_possiveis = [video_pai + e for e in extensoes]
    # id_video_pai = videos_db[videos_db['filename'].isin(pais_possiveis)]
    # id_video_pai = id_video_pai.iloc[0]['id'] if id_video_pai.shape[0] > 0 else None
    # if id_video_pai is None and video_pai in videos_ids:
    #     id_video_pai = videos_ids[video_pai]

    return (id_video_pai, video_pai)


def get_distorcao(sem_extensao):
    global distorcoes_db
    # distorcao = sem_extensao.split("_")
    # distorcao = distorcao[len(distorcao) - 1]
    # distorcao = distorcoes_db[distorcoes_db['slug'] == distorcao]
    # distorcao = distorcao.iloc[0] if distorcao.shape[0] > 0 else None

    saida = None

    for index, dist in distorcoes_db.iterrows():
        distorcao_slug = dist['slug']

        if sem_extensao.lower().startswith(distorcao_slug):
            saida = dist
            break

    return saida


# ------ constantes
extensoes = ['.avi', '.mp4']
q_get_video = "SELECT * FROM videos WHERE filename = %s LIMIT 1"
q_get_distorcao = "SELECT * FROM distorcoes WHERE slug = %s LIMIT 1"

# ------- parsear argumentos
parser = argparse.ArgumentParser()
parser.add_argument("-p", "--path",
                    help="Caminho dos arquivos de vídeo",
                    type=str,
                    dest="path",
                    nargs="?",
                    default=os.getcwd()
                    )
parser.add_argument('-e', '--extensoes',
                    help="Extensões adicionais (padrão: .avi, .mp4)",
                    default=[],
                    nargs="+"
                    )
parser.add_argument('-d', '--debug',
                    help="Ativar debug",
                    action="store_true",
                    dest="debug",
                    default=False
                    )
parser.add_argument('-db', '--database',
                    help="Nome do arquivo de banco de dados sqlite",
                    type=str,
                    dest="database",
                    default="dados.db"
                    )
parser.add_argument("-dp", "--database-path",
                    help="Caminho da base de dados",
                    type=str,
                    dest="database_path",
                    nargs="?",
                    default=os.getcwd()
                    )

args = parser.parse_args()

# configurar logs
if args.debug:
    logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
else:
    logging.basicConfig(stream=sys.stderr, level=logging.INFO)

# ------ config
extensoes = extensoes + args.extensoes
ext_regex = re.compile("|".join(extensoes))
logging.debug("Extensões {}".format(extensoes))


# ------ conectar com banco de dados
database_complete_path = os.path.join(args.database_path, args.database)
if not os.path.exists(database_complete_path):
    logging.error("Banco de dados não encontrado")
    exit(1)
conn = sqlite3.connect(database_complete_path)


# ------ carregar dados
distorcoes_db = []
videos_db = []

distorcoes_db = pd.read_sql_query("SELECT * FROM distorcoes", conn)
videos_db = pd.read_sql_query("SELECT * FROM videos", conn)
videos_list = videos_db['filename'].values.tolist()


# ------ listar videos
videos = os.listdir(args.path)
videos = [video for video in videos if video.endswith(tuple(extensoes))]
logging.info("{} vídeos encontrados".format(len(videos)))
videos_ids = {}


count = 0
# ------ tratar vídeos
for video in videos:
    sem_extensao = ext_regex.sub("", video, count=1)

    id_video_pai, video_pai = get_video_pai(sem_extensao)
    distorcao = get_distorcao(sem_extensao)

    # get video
    video_salvo = True if video in videos_list else False

    if not video_salvo:
        # logging.info("Vídeo não salvo")
        count += 1
        print("\r{}".format(count), end="")

        cursor = conn.cursor()
        cursor.execute("INSERT INTO videos (filename, distorcao_id, video_original) VALUES (?, ?, ?)",
                       (video, int(distorcao['id']) if distorcao is not None else None, int(id_video_pai))
                       )
        conn.commit()
        videos_ids[sem_extensao] = cursor.lastrowid
        cursor.close()
