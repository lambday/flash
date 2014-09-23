test.so: test.h test.i
	swig -c++ -python test.i
	g++ -c test.cpp test_wrap.cxx -fPIC -I. -I/usr/include/python2.7/ -L/usr/lib64/libpython2.7.so
	g++ -shared test_wrap.o -o _test.so
clean:
	rm *.o *.cxx *.so *.py *.pyc
