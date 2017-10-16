#! /bin/bash

transform=jpeg;

ffmpeg -i $1 %05d.png;

for image in $(ls *.png); do
   name=`echo ${image} | cut -d"." -f1`;
   convert ${image} -quality 20% ${name}.jpg;
   convert ${name}.jpg ${name}.png;
   rm ${name}.jpg;
   #echo $name;
done

rm -f $2_${transform}.avi;

ffmpeg -r 25 -i %05d.png -q:v 1 $2_${transform}.avi;

rm *.png;
