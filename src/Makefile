SRC			= flash
INCLUDES	= -I. -I/usr/include/eigen3
OPTS		= -O3 -std=c++11 -fPIC
STATISTICS	= $(SRC)/statistics
INTERNALS	= $(STATISTICS)/internals
OBJECTS		= $(STATISTICS)/HypothesisTest.o $(STATISTICS)/TwoSampleTest.o $(STATISTICS)/IndependenceTest.o \
			  $(INTERNALS)/FetcherPolicy.o $(INTERNALS)/Permutators.o $(INTERNALS)/PermutationPolicy.o \
			  $(STATISTICS)/StreamingTwoSampleTest.o $(INTERNALS)/FetcherFactory.o $(INTERNALS)/PermutatorFactory.o \
			  $(STATISTICS)/StreamingIndependenceTest.o
INTERNAL	= $(INTERNALS)/Features.h $(INTERNALS)/DataManager.h $(INTERNALS)/FetcherPolicy.h \
			  $(INTERNALS)/PermutationPolicy.h $(INTERNALS)/TestTypes.h $(INTERNALS)/Permutators.h

libflash.so: $(OBJECTS)
	g++ $(OPTS) -shared $(OBJECTS) $(INCLUDES) -o libflash.so -lshogun
$(STATISTICS)/HypothesisTest.o: $(STATISTICS)/HypothesisTest.h $(STATISTICS)/HypothesisTest.cpp $(INTERNALS)/DataManager.h
	g++ $(OPTS) -c $(STATISTICS)/HypothesisTest.cpp $(INCLUDES) -o $(STATISTICS)/HypothesisTest.o
$(STATISTICS)/TwoSampleTest.o: $(STATISTICS)/TwoSampleTest.h $(STATISTICS)/TwoSampleTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/TwoSampleTest.cpp $(INCLUDES) -o $(STATISTICS)/TwoSampleTest.o
$(STATISTICS)/StreamingTwoSampleTest.o: $(STATISTICS)/StreamingTwoSampleTest.h $(STATISTICS)/StreamingTwoSampleTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/StreamingTwoSampleTest.cpp $(INCLUDES) -o $(STATISTICS)/StreamingTwoSampleTest.o
$(STATISTICS)/IndependenceTest.o: $(STATISTICS)/IndependenceTest.h $(STATISTICS)/IndependenceTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/IndependenceTest.cpp $(INCLUDES) -o $(STATISTICS)/IndependenceTest.o
$(STATISTICS)/StreamingIndependenceTest.o: $(STATISTICS)/StreamingIndependenceTest.h $(STATISTICS)/StreamingIndependenceTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/StreamingIndependenceTest.cpp $(INCLUDES) -o $(STATISTICS)/StreamingIndependenceTest.o
$(INTERNALS)/FetcherPolicy.o: $(INTERNALS)/FetcherPolicy.h $(INTERNALS)/FetcherPolicy.cpp
	g++ $(OPTS) -c $(INTERNALS)/FetcherPolicy.cpp $(INCLUDES) -o $(INTERNALS)/FetcherPolicy.o
$(INTERNALS)/FetcherFactory.o: $(INTERNALS)/FetcherFactory.h $(INTERNALS)/FetcherFactory.cpp
	g++ $(OPTS) -c $(INTERNALS)/FetcherFactory.cpp $(INCLUDES) -o $(INTERNALS)/FetcherFactory.o
$(INTERNALS)/Permutators.o: $(INTERNALS)/Permutators.h $(INTERNALS)/Permutators.cpp
	g++ $(OPTS) -c $(INTERNALS)/Permutators.cpp $(INCLUDES) -o $(INTERNALS)/Permutators.o
$(INTERNALS)/PermutatorFactory.o: $(INTERNALS)/PermutatorFactory.h $(INTERNALS)/PermutatorFactory.cpp
	g++ $(OPTS) -c $(INTERNALS)/PermutatorFactory.cpp $(INCLUDES) -o $(INTERNALS)/PermutatorFactory.o
$(INTERNALS)/PermutationPolicy.o: $(INTERNALS)/PermutationPolicy.h $(INTERNALS)/PermutationPolicy.cpp \
		$(INTERNALS)/TestTypes.h $(INTERNALS)/Permutators.h
	g++ $(OPTS) -c $(INTERNALS)/PermutationPolicy.cpp $(INCLUDES) -o $(INTERNALS)/PermutationPolicy.o
clean:
	rm $(STATISTICS)/*.o $(INTERNALS)/*.o libflash.so
