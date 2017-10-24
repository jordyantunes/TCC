#! /bin/bash

#Compilando a biblioteca do KLT e o programa principal
cd klt
cp libklt.a ../ 
make
cd ..
gcc -o main main.c motion.c ./PNG/png.c ./UTIL/image.c -Lklt -lklt -lm

#Rodando o programa para um par de frames consecutivos. Os resultados sção gravados tbm em desc.txt
./main frames/00004.png frames/00005.png desc.txt 
