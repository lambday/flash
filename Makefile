OPTS	 = -O3 -std=c++14
INCLUDES = -Isrc -I/usr/include/eigen3
LIBS	 = -Lsrc -lflash -lshogun
a.out: test_main.cpp src/libflash.so
	g++ test_main.cpp $(OPTS) $(INCLUDES) $(LIBS)
clean:
	rm a.out
