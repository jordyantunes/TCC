#! /bin/bash

transform=text;

ffmpeg -i $1 %05d.png;

FILES=$(ls *.png | wc -l);
CNT=1;
T=$(shuf -n 5 words | tr '\n' ' ');

first_image=`ls *.png | head -n 1`
h=$(((`identify -format %h ${first_image}` / 2) * 8 / 10 ))

for image in $(ls *.png); do
   echo -e -n "\r${CNT}/${FILES}";

   name=`echo ${image} | cut -d"." -f1`;

   if ((CNT%20 == 0)); then
   		R=$(( ( RANDOM % 7 )  + 1 ))
   		T=$(shuf -n ${R} words | tr '\n' ' ');
   fi
   let 'CNT+=1'

   convert -pointsize 50 -weight Bold -fill yellow -gravity center -draw "text 0,${h} \"${T}\" " ${image} ${name}.png;
   # convert -pointsize 30 -fill red -draw "text 10,120 \"${T}\" "  ${name}.png ${name}.png;
   #echo $name;
done

rm -f $2_${transform}.avi;

ffmpeg -r 25 -i %05d.png -q:v 1 $2_${transform}.avi;

rm *.png;
