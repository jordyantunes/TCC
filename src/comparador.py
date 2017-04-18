import sys
import numpy
import cv2
import medidaOrdinal
import gradiente

def main():
	# descritor_ordinal = medidaOrdinal.loadResource(sys.argv[1], "../output/gato_ordinal.txt")
	# descritor_gradiente = gradiente.loadResource(sys.argv[1], "../output/gato_gradiente.txt")
	# descritor_original_ordinal = medidaOrdinal.loadResource(sys.argv[1], "../output/gato_original_ordinal.txt")

	descritor_original_ordinal = open(sys.argv[1], "r")
	descritor_ordinal = open (sys.argv[2], "r")

	descritor_tmp = descritor_ordinal.readlines()
	descritor_o_array = descritor_original_ordinal.readlines()

	
	for key in range(len(descritor_o_array)):
		original = float(descritor_o_array[key])
		for chave, d in enumerate(descritor_tmp):
			if original != float(d):
				print original
				print d
				break
			else:
				tam = len(descritor_tmp)
				if chave == tam-1:
					print "achemo o barnabeh"
					print key
					return 1


 


if __name__ == '__main__':
	main()