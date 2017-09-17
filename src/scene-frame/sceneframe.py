import numpy as np
import cv2
import sys

def is_not_black(frame):
	return (np.average(frame) > 20)

def hamming_distance(s1, s2):
    return sum(ch1 != ch2 for ch1, ch2 in zip(s1, s2))

def is_scene_frame(result, analyzed):

	# Ugly bypass for the first value on "result"
	if (type(result[-1]) is np.ndarray):
		return (hamming_distance(result[-1], analyzed) > 490)
	else:
		return (hamming_distance(result, analyzed) > 490)

def calculate_threshold(elements, type):
	if (type == "mean"):
		values = [abs(x - 128) for x in elements]
		values.sort()
		return values[35] # 144*0.25
	else:
		values = [abs(x) for x in elements]
		values.sort()
		return values[143] # 576*0.25

def quaternize(elements, type):
	threshold = calculate_threshold(elements, type)
	
	if (type == "mean"):
		elements = [e - 128 for e in elements]

	for i, e in enumerate(elements):
		if (e > threshold):
			elements[i] = 3
		elif (e > 0 and e <= threshold):
			elements[i] = 2
		elif ((threshold*(-1) < e) and e <= 0):
			elements[i] = 1
		else:
			elements[i] = 0
			
	return elements

def generate_fingerprint(frame):
	width = 11
	height = 9

	little_frames = []
	mean_elements = []
	diff_elements = []
	
	fingerprint = []
	
	# Get 11x9 elements
	for r in range(0, frame.shape[0], height):
		for c in range(0, frame.shape[1], width):
			window = frame[r:r+height,c:c+width]
			little_frames.append(window[:,:,1])

	for f in little_frames:
		# Get mean elements
		mean_elements.append(np.average(f))

		# Get 4 differential elements
		a_b = np.average(f[0:2, 0:5]) - np.average(f[2:4, 0:5])
		diff_elements.append(a_b)

		c_d = np.average(f[0:4, 5:8]) - np.average(f[0:4, 8:11])
		diff_elements.append(c_d)

		e_f = np.average(f[4:9, 0:2]) - np.average(f[4:9, 2:5])
		diff_elements.append(e_f)

		g_h = np.average(f[4:6, 5:11]) - np.average(f[6:9, 5:11])
		diff_elements.append(g_h)

	fingerprint.extend(quaternize(mean_elements, "mean"))
	fingerprint.extend(quaternize(diff_elements, "diff"))

	return fingerprint

def process_frame(frame):
	# Get central area
	width, height = frame.shape[:2]

	if (width >= 144*3 and height >= 176*3):
		x = (width - 360)/2
		y = (height - 440)/2

		processed_frame = frame[y:y+440, x:x+360]	
	else:
		x = (width - 144)/2
		y = (height - 176)/2

		processed_frame = frame[y:176, x:144]
	
	# Resize image
	processed_frame = cv2.resize(processed_frame, (132,108), fx=0, fy=0)
	
	# HLS space color
	processed_frame = cv2.cvtColor(processed_frame,cv2.COLOR_BGR2HLS)

	# Gaussian low pass filter
	processed_frame = cv2.GaussianBlur(processed_frame, (3,3), 0.95)
	
	return processed_frame

def sceneFrame(video):
	result = np.array([])
	frame_fingerprint = np.array([])

	while (video.isOpened()):
		ret, frame = video.read()

		if (ret == False):
			break

		processed_frame = process_frame(frame)

		cv2.imshow("L", processed_frame[:, :, 1])

		if (is_not_black(processed_frame)):
			frame_fingerprint = generate_fingerprint(processed_frame)

		if (len(result) == 0):
			result = np.array(frame_fingerprint)

		elif (is_scene_frame(result, frame_fingerprint)):
			result = np.vstack((result, frame_fingerprint))

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break
	
	return result 

def main():
	cap = cv2.VideoCapture(sys.argv[1])
	result = sceneFrame(cap)

	np.set_printoptions(threshold='nan')
	print len(result)

	cap.release()

if __name__ == "__main__":
	main()