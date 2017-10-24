#! /bin/bash

transform=text;

ffmpeg -i $1 %05d.png;

for image in $(ls *.png); do
   name=`echo ${image} | cut -d"." -f1`;
   convert -pointsize 30 -fill red -draw 'text 10,70 "Testing descriptor for several distortions and effects" ' ${image} ${name}.png;
   convert -pointsize 30 -fill red -draw 'text 10,120 "Testing descriptor for several distortions and effects" ' ${name}.png ${name}.png;
   #echo $name;
done

rm -f $2_${transform}.avi;

ffmpeg -r 25 -i %05d.png -q:v 1 $2_${transform}.avi;

rm *.png;
