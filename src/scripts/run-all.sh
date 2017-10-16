#! /bin/bash

video=bs1_25fps;

#Converting YUV to MP4:
ffmpeg -s 768x432 -r 25 -i ${video}.yuv -q:v 1 bs1_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs2_25fps.yuv -q:v 1 bs2_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs3_25fps.yuv -q:v 1 bs3_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs4_25fps.yuv -q:v 1 bs4_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs5_25fps.yuv -q:v 1 bs5_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs6_25fps.yuv -q:v 1 bs6_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs7_25fps.yuv -q:v 1 bs7_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs8_25fps.yuv -q:v 1 bs8_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs9_25fps.yuv -q:v 1 bs9_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs10_25fps.yuv -q:v 1 bs10_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs11_25fps.yuv -q:v 1 bs11_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs12_25fps.yuv -q:v 1 bs12_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs13_25fps.yuv -q:v 1 bs13_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs14_25fps.yuv -q:v 1 bs14_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs15_25fps.yuv -q:v 1 bs15_25fps.mp4;
ffmpeg -s 768x432 -r 25 -i bs16_25fps.yuv -q:v 1 bs16_25fps.mp4;

#Transformations:
./trans_big.sh bs1_25fps.mp4 ${video};
./trans_wmark.sh bs1_25fps.mp4 ${video};
./trans_text.sh bs1_25fps.mp4 ${video};
./trans_small.sh bs1_25fps.mp4 ${video};
./trans_rotate.sh bs1_25fps.mp4 ${video};
./trans_jpeg.sh bs1_25fps.mp4 ${video};
./trans_frame.sh bs1_25fps.mp4 ${video};
./trans_flop.sh bs1_25fps.mp4 ${video};
./trans_fast1.sh bs1_25fps.mp4 ${video};
./trans_fast2.sh bs1_25fps.mp4 ${video};
./trans_crop.sh bs1_25fps.mp4 ${video};
./trans_border.sh bs1_25fps.mp4 ${video};
./trans_blur.sh bs1_25fps.mp4 ${video};
./trans_color.sh bs1_25fps.mp4 ${video};
