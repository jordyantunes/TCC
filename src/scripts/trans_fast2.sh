#! /bin/bash

transform=fast2;

ffmpeg -i $1 %05d.png;

ffmpeg -i %05d.png -q:v 1 -vf "setpts=(1/4)*PTS" $2_${transform}.avi;

rm *.png;
