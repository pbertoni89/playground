#!/bin/sh

rm *.o

# gcc -c my_main.c
# gcc -o my_main my_main.c pb_simplest_f.a

make -f pb_simplest_f_rtw.mk

gcc -o my_main my_main.c pb_simplest_f.a
