import os
import shutil
import itertools
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('-op',
                    '--origin-path',
                    type=str,
                    dest="origin_path",
                    default=os.getcwd())
parser.add_argument('-dp',
                    '--destination-path',
                    type=str,
                    dest="destination_path",
                    default=os.getcwd())
parser.add_argument('-sl', '--symlink',
                    action="store_true",
                    help="Criar link simbolico ao inves de copiar o video",
                    dest="create_symlink")
args = parser.parse_args()

extensios = ['.avi', '.mp4']

count = 0

for root, dirs, files in os.walk(args.origin_path):
    videos = [(os.path.join(args.destination_path, f), os.path.join(root, f)) for f in files]
    for d, v in videos:
        count += 1
        print("\r{}".format(count), end='')

        if not args.create_symlink:
            shutil.copy(v, d)
        else:
            os.symlink(v, d)
