_modcomputation.so: ComputeFoo.o ComputeCube.o computation.i
	swig -c++ -python computation.i
	g++ -O3 -std=c++11 -c computation_wrap.cxx -fPIC -I. -I/usr/include/python2.7/ -L/usr/lib64/libpython2.7.so
	g++ -O3 -std=c++11 -shared ComputeFoo.o ComputeCube.o computation_wrap.o -o _modcomputation.so
computation.so: ComputeFoo.o ComputeCube.o
	g++ -O3 -std=c++11 -shared ComputeFoo.o ComputeCube.o -o computation.so
ComputeFoo.o: ComputeFoo.h ComputeFoo.cpp Computation.h
	g++ -O3 -std=c++11 -c ComputeFoo.cpp -fPIC -I.
ComputeCube.o: ComputeCube.h ComputeCube.cpp Computation.h
	g++ -O3 -std=c++11 -c ComputeCube.cpp -fPIC -I.
test: computation.so
	g++ -O3 -std=c++11 test.cpp computation.so -I. -o test
clean:
	rm *.o *.cxx *.so modcomputation.py *.pyc test
