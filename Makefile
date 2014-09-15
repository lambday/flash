all: test_data_main.cpp test_data.h
	g++ -O3 -std=c++0x -I/usr/include/eigen3 -I. test_data_main.cpp -lshogun
clean:
	rm *.out
