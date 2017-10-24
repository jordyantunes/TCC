#! /bin/bash

transform=crop;

ffmpeg -i $1 %05d.png;

for image in $(ls *.png); do
   name=`echo ${image} | cut -d"." -f1`;
   convert -crop 620x338+107+107 ${image} ${name}.png;
   #echo $name;
done

rm -f $2_${transform}.avi;

ffmpeg -r 25 -i %05d.png -q:v 1 $2_${transform}.avi;

rm *.png;
