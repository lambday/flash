#!/bin/sh

cd src && make clean ; make -j4 && cd .. && make -j4 && ./a.out
