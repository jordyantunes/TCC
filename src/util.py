import argparse
import pandas as pd
import sqlite3
import logging
import sys
import os

# ------- arguments
parser = argparse.ArgumentParser()
parser.add_argument('-vp', '--video-path',
                    help="Caminho dos vídeos",
                    type=str, dest="video_path")
parser.add_argument('-fp', '--fingerprint-path',
                    help="Caminho das assinaturas",
                    type=str, dest="fingerprint_path")
parser.add_argument('-df', '--database-file',
                    help="Caminho do banco de dados",
                    type=str, dest="database_filename")
args = parser.parse_args()

logging.basicConfig(stream=sys.stdout, level=logging.INFO)

if not os.path.exists(args.database_filename):
    logging.error("Banco de dados não encontrado")
    exit(1)

conn = sqlite3.connect(args.database_filename)
videos = pd.read_sql_query("SELECT * FROM videos", conn)
algoritmos = pd.read_sql_query("SELECT * FROM algoritmos", conn)
distorcoes = pd.read_sql_query("SELECT * FROM distorcoes", conn)
comparadores = pd.read_sql_query("SELECT * FROM comparadores", conn)
assinaturas = pd.read_sql_query("SELECT * FROM assinaturas", conn)
