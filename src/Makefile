SRC			= flash
INCLUDES	= -I.
OPTS		= -O3 -std=c++14 -fPIC
STATISTICS	= $(SRC)/statistics
INTERNALS	= $(STATISTICS)/internals
OBJECTS		= $(STATISTICS)/HypothesisTest.o $(STATISTICS)/TwoDistributionTest.o $(STATISTICS)/TwoSampleTest.o \
			  $(STATISTICS)/IndependenceTest.o $(STATISTICS)/OneDistributionTest.o \
			  $(INTERNALS)/DataFetcherFactory.o $(INTERNALS)/DataFetcher.o $(INTERNALS)/StreamingDataFetcher.o \
			  $(INTERNALS)/NextSamples.o $(INTERNALS)/InitPerFeature.o $(INTERNALS)/BlockwiseDetails.o \
			  $(INTERNALS)/DataManager.o $(INTERNALS)/InitPerKernel.o $(INTERNALS)/KernelManager.o
INTERNAL	= $(INTERNALS)/DataFetcherFactory.o $(INTERNALS)/DataFetcher.o $(INTERNALS)/StreamingDataFetcher.o \
			  $(INTERNALS)/NextSamples.o $(INTERNALS)/InitPerFeature.o $(INTERNALS)/BlockwiseDetails.o \
			  $(INTERNALS)/DataManager.o $(INTERNALS)/InitPerKernel.o $(INTERNALS)/KernelManager.o
libflash.so: $(OBJECTS)
	g++ $(OPTS) -shared $(OBJECTS) $(INCLUDES) -o libflash.so -lshogun
	rm $(STATISTICS)/*.o $(INTERNALS)/*.o
$(STATISTICS)/HypothesisTest.o: $(STATISTICS)/HypothesisTest.h $(STATISTICS)/HypothesisTest.cpp
	g++ $(OPTS) -c $(STATISTICS)/HypothesisTest.cpp $(INCLUDES) -o $(STATISTICS)/HypothesisTest.o
$(STATISTICS)/OneDistributionTest.o: $(STATISTICS)/OneDistributionTest.h $(STATISTICS)/OneDistributionTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/OneDistributionTest.cpp $(INCLUDES) -o $(STATISTICS)/OneDistributionTest.o
$(STATISTICS)/TwoDistributionTest.o: $(STATISTICS)/TwoDistributionTest.h $(STATISTICS)/TwoDistributionTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	g++ $(OPTS) -c $(STATISTICS)/TwoDistributionTest.cpp $(INCLUDES) -o $(STATISTICS)/TwoDistributionTest.o
$(STATISTICS)/TwoSampleTest.o: $(STATISTICS)/TwoSampleTest.h $(STATISTICS)/TwoSampleTest.cpp
	g++ $(OPTS) -c $(STATISTICS)/TwoSampleTest.cpp $(INCLUDES) -o $(STATISTICS)/TwoSampleTest.o
$(STATISTICS)/IndependenceTest.o: $(STATISTICS)/IndependenceTest.h $(STATISTICS)/IndependenceTest.cpp
	g++ $(OPTS) -c $(STATISTICS)/IndependenceTest.cpp $(INCLUDES) -o $(STATISTICS)/IndependenceTest.o
$(INTERNALS)/DataManager.o: $(INTERNALS)/DataManager.h $(INTERNALS)/DataManager.cpp $(INTERNALS)/InitPerFeature.h
	g++ $(OPTS) -c $(INTERNALS)/DataManager.cpp $(INCLUDES) -o $(INTERNALS)/DataManager.o
$(INTERNALS)/InitPerFeature.o: $(INTERNALS)/InitPerFeature.h $(INTERNALS)/InitPerFeature.cpp
	g++ $(OPTS) -c $(INTERNALS)/InitPerFeature.cpp $(INCLUDES) -o $(INTERNALS)/InitPerFeature.o
$(INTERNALS)/NextSamples.o: $(INTERNALS)/NextSamples.h $(INTERNALS)/NextSamples.cpp
	g++ $(OPTS) -c $(INTERNALS)/NextSamples.cpp $(INCLUDES) -o $(INTERNALS)/NextSamples.o
$(INTERNALS)/DataFetcherFactory.o: $(INTERNALS)/DataFetcherFactory.h $(INTERNALS)/DataFetcherFactory.cpp
	g++ $(OPTS) -c $(INTERNALS)/DataFetcherFactory.cpp $(INCLUDES) -o $(INTERNALS)/DataFetcherFactory.o
$(INTERNALS)/BlockwiseDetails.o: $(INTERNALS)/BlockwiseDetails.h $(INTERNALS)/BlockwiseDetails.cpp
	g++ $(OPTS) -c $(INTERNALS)/BlockwiseDetails.cpp $(INCLUDES) -o $(INTERNALS)/BlockwiseDetails.o
$(INTERNALS)/DataFetcher.o: $(INTERNALS)/DataFetcher.h $(INTERNALS)/DataFetcher.cpp
	g++ $(OPTS) -c $(INTERNALS)/DataFetcher.cpp $(INCLUDES) -o $(INTERNALS)/DataFetcher.o
$(INTERNALS)/StreamingDataFetcher.o: $(INTERNALS)/StreamingDataFetcher.h $(INTERNALS)/StreamingDataFetcher.cpp
	g++ $(OPTS) -c $(INTERNALS)/StreamingDataFetcher.cpp $(INCLUDES) -o $(INTERNALS)/StreamingDataFetcher.o
$(INTERNALS)/KernelManager.o: $(INTERNALS)/KernelManager.h $(INTERNALS)/KernelManager.cpp $(INTERNALS)/InitPerKernel.h
	g++ $(OPTS) -c $(INTERNALS)/KernelManager.cpp $(INCLUDES) -o $(INTERNALS)/KernelManager.o
$(INTERNALS)/InitPerKernel.o: $(INTERNALS)/InitPerKernel.h $(INTERNALS)/InitPerKernel.cpp
	g++ $(OPTS) -c $(INTERNALS)/InitPerKernel.cpp $(INCLUDES) -o $(INTERNALS)/InitPerKernel.o
clean:
	rm $(STATISTICS)/*.o $(INTERNALS)/*.o libflash.so
