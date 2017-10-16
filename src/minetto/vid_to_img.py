import subprocess
import numpy as np
import cv2
import argparse
import os
import errno
import platform
import subprocess



def main():
	parser = argparse.ArgumentParser(description="Comparador de videos usando assinatura")
	parser.add_argument('video', type=str, help='Video')
	parser.add_argument('--diretorio', type=str, help="Diretorio para salvar frames", default="./out/")
	
	args 	= parser.parse_args()
	video 	= cv2.VideoCapture(args.video)

	print("Video aberto")

	"""
		Criar diretorio se nao existe
	"""
	if not os.path.exists(os.path.dirname(args.diretorio)):
		try:
		    os.makedirs(os.path.dirname(args.diretorio))
		except OSError as exc: # Guard against race condition
		    if exc.errno != errno.EEXIST:
		        raise

    # contador para dar nome aos arquivos
	counter = 0

	while (video.isOpened()):
		ret_video, frame_video = video.read()
		
		counter = counter + 1

		# checa se frame é vazio
		if np.shape(frame_video) == ():
			break

		saida = args.diretorio + "frame_%d.png"%counter
		anterior = args.diretorio + "frame_%d.png"%(counter - 1)
		cv2.imwrite(saida, frame_video)

		if counter != 1 :
			print("usando alg do minettao")
			print(subprocess.check_call(['bash.exe', '-c', "./camera-motion/main %s %s test.txt".format(anterior, saida)], shell=True))

	video.release()
	print("Terminado")

if __name__ == '__main__':
	main()