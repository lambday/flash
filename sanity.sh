#!/bin/sh

cd src && make clean ; make -j4 && cd .. && make -j4 && valgrind ./a.out --leak-check=full --track-origins=yes
