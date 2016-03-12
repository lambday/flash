#!/bin/sh

cd src && make clean ; make -j4 && cd .. && make -j4
valgrind tests/DataFetcherTest --leak-check=full --track-origins=yes
valgrind tests/StreamingDataFetcherTest --leak-check=full --track-origins=yes
valgrind tests/DataFetcherFactoryTest --leak-check=full --track-origins=yes
valgrind tests/InitPerFeatureTest --leak-check=full --track-origins=yes
valgrind tests/DataManagerTest --leak-check=full --track-origins=yes
valgrind tests/KernelManagerTest --leak-check=full --track-origins=yes
