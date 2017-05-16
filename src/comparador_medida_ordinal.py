import sys
import numpy as np
import cv2
import math
import medidaOrdinal

def main():
	needle 		= cv2.VideoCapture(sys.argv[1])
	haystack 	= cv2.VideoCapture(sys.argv[2])

	needle_buffer = np.array([])
	neddle_fingerprint_buffer = np.array([])

	frames_matched = 0
	haystack_frame_count = 0

	needle_frame_count = needle.get(cv2.CAP_PROP_FRAME_COUNT)

	while (haystack.isOpened()):

		haystack_frame_count += 1

		ret_haystack, frame_haystack = haystack.read()
		ret_needle, frame_needle = needle.read()
		
		if (np.shape(frame_haystack) == ()) or (np.shape(frame_needle) == ()):
			break

		if frames_matched >= needle_frame_count:
			print "Video encontrado no intervalo {}-{}".format(haystack_frame_count - frames_matched, haystack_frame_count)
			break


		frame_haystack 	= medidaOrdinal.pre_process(frame_haystack,500,500)
		frame_needle 	= medidaOrdinal.pre_process(frame_needle,500,500)

		cv2.imshow("Haystack", frame_haystack)
		cv2.imshow("Needle", frame_needle)

		descritor_haystack = medidaOrdinal.frame_to_fingerprint(frame_haystack,500,500,5,5)
		descritor_needle = medidaOrdinal.frame_to_fingerprint(frame_needle,500,500,5,5)
		

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

		if not (descritor_needle == descritor_haystack).all():
			needle.release()
			needle = cv2.VideoCapture(sys.argv[1])
			frames_matched = 0
		else :
			frames_matched += 1
			print "Frames matched {}".format(frames_matched)


	haystack.release()
	needle.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	main()