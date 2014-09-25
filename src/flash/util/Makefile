test: test.o type.o
	g++ -O3 -std=c++11 test.o type.o -I. -o test
test.o: test.cpp type.hpp
	g++ -O3 -std=c++11 -c test.cpp -I.
type.o: type.cpp type.hpp
	g++ -O3 -std=c++11 -c type.cpp -I.
clean:
	rm *.o test
