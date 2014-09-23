_computation.so: Computation.o computation.i
	swig -c++ -python computation.i
	g++ -O3 -std=c++11 -c computation_wrap.cxx -fPIC -I. -I/usr/include/python2.7/ -L/usr/lib64 -lpython2.7
	g++ -O3 -std=c++11 -shared Computation.o computation_wrap.o -o _modcomputation.so
computation.so: Computation.o
	g++ -O3 -std=c++11 -shared Computation.o -o computation.so
Computation.o: Computation.h Computation.cpp ComputeFoo.h ComputeCube.h
	g++ -O3 -std=c++11 -c Computation.cpp -fPIC -I.
test: computation.so
	g++ -O3 -std=c++11 test.cpp computation.so -I. -o test
clean:
	rm *.o *.cxx *.so modcomputation.py *.pyc test
