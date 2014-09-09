all: main.cpp test_data.h
	g++ -O3 -std=c++0x -I/usr/include/eigen3 -I. main.cpp -lshogun
clean:
	rm *.out
