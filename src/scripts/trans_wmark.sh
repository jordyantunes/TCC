#! /bin/bash

transform=wmark;

ffmpeg -i $1 %05d.png;

for image in $(ls *.png); do
   name=`echo ${image} | cut -d"." -f1`;
   #composite -dissolve 20 -tile wmark.PNG 00023.png out.png;
   #composite -dissolve 20% -gravity center wmark2.PNG 00023.png out.png;
   convert -size 140x80 -pointsize 20 xc:none -fill grey -gravity NorthWest -draw "text 10,10 'Copyright'" -gravity SouthEast -draw "text 5,15 'Copyright'" miff:- | composite -dissolve 35% -tile - ${image} ${name}.png;
   #echo $name;
done

rm -f $2_${transform}.avi;

ffmpeg -r 25 -i %05d.png -q:v 1 $2_${transform}.avi;

rm *.png;
