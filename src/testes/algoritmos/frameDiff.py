import numpy as np
import cv2
import sys


def frameDiff(video):
    descritores = np.array([[0, 0]])

    rec_1, frame_1 = video.read()
    frame_1 = cv2.normalize(frame_1, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)

    while(video.isOpened()):
        rec_2, frame_2 = video.read()
        frame_2 = cv2.normalize(frame_2,
            None,
            alpha=0,
            beta=1,
            norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
        if rec_2:
            y = np.sum(frame_1)
            dY = np.sum(np.absolute(np.subtract(frame_1, frame_2)))
            rec_1, frame_1 = video.read()
            frame_1 = cv2.normalize(frame_1, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
            descritores = np.append(descritores, np.array([[dY, y]]), axis=0)
            # print(descritores)
        else:
            break

    return descritores


def run(video_path, filename):
    cap = cv2.VideoCapture(video_path)
    resultados = frameDiff(cap)
    np.savetxt("{}_framediff.csv".format(filename), resultados, delimiter=",")
    cap.release()


def main():
    run(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    main()
