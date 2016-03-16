CC			= ccache g++
SRC			= flash
INCLUDES	= -I. -I/usr/include/eigen3
OPTS		= -O3 -std=c++14 -fPIC -fopenmp
STATISTICS	= $(SRC)/statistics
INTERNALS	= $(STATISTICS)/internals
OBJECTS		= $(STATISTICS)/HypothesisTest.o $(STATISTICS)/TwoDistributionTest.o $(STATISTICS)/TwoSampleTest.o \
			  $(STATISTICS)/IndependenceTest.o $(STATISTICS)/OneDistributionTest.o $(STATISTICS)/MMD.o \
			  $(INTERNALS)/DataFetcherFactory.o $(INTERNALS)/DataFetcher.o $(INTERNALS)/StreamingDataFetcher.o \
			  $(INTERNALS)/NextSamples.o $(INTERNALS)/InitPerFeature.o $(INTERNALS)/BlockwiseDetails.o $(INTERNALS)/DataManager.o \
			  $(INTERNALS)/InitPerKernel.o $(INTERNALS)/KernelManager.o $(INTERNALS)/ComputationManager.o \
			  $(INTERNALS)/mmd/UnbiasedFull.o $(INTERNALS)/mmd/WithinBlockDirect.o $(INTERNALS)/mmd/WithinBlockPermutation.o \
			  $(INTERNALS)/mmd/UnbiasedIncomplete.o $(INTERNALS)/mmd/BiasedFull.o $(INTERNALS)/mmd/FullDirect.o
INTERNAL	= $(INTERNALS)/DataFetcherFactory.o $(INTERNALS)/DataFetcher.o $(INTERNALS)/StreamingDataFetcher.o \
			  $(INTERNALS)/NextSamples.o $(INTERNALS)/InitPerFeature.o $(INTERNALS)/BlockwiseDetails.o $(INTERNALS)/DataManager.o \
			  $(INTERNALS)/InitPerKernel.o $(INTERNALS)/KernelManager.o $(INTERNALS)/ComputationManager.o \
			  $(INTERNALS)/mmd/UnbiasedFull.o $(INTERNALS)/mmd/WithinBlockDirect.o $(INTERNALS)/mmd/WithinBlockPermutation.o \
	          $(INTERNALS)/mmd/UnbiasedIncomplete.o $(INTERNALS)/mmd/BiasedFull.o $(INTERNALS)/mmd/FullDirect.o
libflash.so: $(OBJECTS)
	$(CC) $(OPTS) -shared $(OBJECTS) $(INCLUDES) -o libflash.so -lshogun
	rm $(OBJECTS)
$(STATISTICS)/HypothesisTest.o: $(STATISTICS)/HypothesisTest.h $(STATISTICS)/HypothesisTest.cpp
	$(CC) $(OPTS) -c $(STATISTICS)/HypothesisTest.cpp $(INCLUDES) -o $(STATISTICS)/HypothesisTest.o
$(STATISTICS)/OneDistributionTest.o: $(STATISTICS)/OneDistributionTest.h $(STATISTICS)/OneDistributionTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	$(CC) $(OPTS) -c $(STATISTICS)/OneDistributionTest.cpp $(INCLUDES) -o $(STATISTICS)/OneDistributionTest.o
$(STATISTICS)/TwoDistributionTest.o: $(STATISTICS)/TwoDistributionTest.h $(STATISTICS)/TwoDistributionTest.cpp \
		$(STATISTICS)/HypothesisTest.h $(INTERNALS)/TestTypes.h
	$(CC) $(OPTS) -c $(STATISTICS)/TwoDistributionTest.cpp $(INCLUDES) -o $(STATISTICS)/TwoDistributionTest.o
$(STATISTICS)/TwoSampleTest.o: $(STATISTICS)/TwoSampleTest.h $(STATISTICS)/TwoSampleTest.cpp
	$(CC) $(OPTS) -c $(STATISTICS)/TwoSampleTest.cpp $(INCLUDES) -o $(STATISTICS)/TwoSampleTest.o
$(STATISTICS)/IndependenceTest.o: $(STATISTICS)/IndependenceTest.h $(STATISTICS)/IndependenceTest.cpp
	$(CC) $(OPTS) -c $(STATISTICS)/IndependenceTest.cpp $(INCLUDES) -o $(STATISTICS)/IndependenceTest.o
$(STATISTICS)/MMD.o: $(STATISTICS)/MMD.h $(STATISTICS)/MMD.cpp
	$(CC) $(OPTS) -c $(STATISTICS)/MMD.cpp $(INCLUDES) -o $(STATISTICS)/MMD.o
$(INTERNALS)/DataManager.o: $(INTERNALS)/DataManager.h $(INTERNALS)/DataManager.cpp $(INTERNALS)/InitPerFeature.h
	$(CC) $(OPTS) -c $(INTERNALS)/DataManager.cpp $(INCLUDES) -o $(INTERNALS)/DataManager.o
$(INTERNALS)/InitPerFeature.o: $(INTERNALS)/InitPerFeature.h $(INTERNALS)/InitPerFeature.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/InitPerFeature.cpp $(INCLUDES) -o $(INTERNALS)/InitPerFeature.o
$(INTERNALS)/NextSamples.o: $(INTERNALS)/NextSamples.h $(INTERNALS)/NextSamples.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/NextSamples.cpp $(INCLUDES) -o $(INTERNALS)/NextSamples.o
$(INTERNALS)/DataFetcherFactory.o: $(INTERNALS)/DataFetcherFactory.h $(INTERNALS)/DataFetcherFactory.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/DataFetcherFactory.cpp $(INCLUDES) -o $(INTERNALS)/DataFetcherFactory.o
$(INTERNALS)/BlockwiseDetails.o: $(INTERNALS)/BlockwiseDetails.h $(INTERNALS)/BlockwiseDetails.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/BlockwiseDetails.cpp $(INCLUDES) -o $(INTERNALS)/BlockwiseDetails.o
$(INTERNALS)/DataFetcher.o: $(INTERNALS)/DataFetcher.h $(INTERNALS)/DataFetcher.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/DataFetcher.cpp $(INCLUDES) -o $(INTERNALS)/DataFetcher.o
$(INTERNALS)/StreamingDataFetcher.o: $(INTERNALS)/StreamingDataFetcher.h $(INTERNALS)/StreamingDataFetcher.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/StreamingDataFetcher.cpp $(INCLUDES) -o $(INTERNALS)/StreamingDataFetcher.o
$(INTERNALS)/KernelManager.o: $(INTERNALS)/KernelManager.h $(INTERNALS)/KernelManager.cpp $(INTERNALS)/InitPerKernel.h
	$(CC) $(OPTS) -c $(INTERNALS)/KernelManager.cpp $(INCLUDES) -o $(INTERNALS)/KernelManager.o
$(INTERNALS)/InitPerKernel.o: $(INTERNALS)/InitPerKernel.h $(INTERNALS)/InitPerKernel.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/InitPerKernel.cpp $(INCLUDES) -o $(INTERNALS)/InitPerKernel.o
$(INTERNALS)/ComputationManager.o: $(INTERNALS)/ComputationManager.h $(INTERNALS)/ComputationManager.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/ComputationManager.cpp $(INCLUDES) -o $(INTERNALS)/ComputationManager.o
$(INTERNALS)/mmd/BiasedFull.o: $(INTERNALS)/mmd/BiasedFull.h $(INTERNALS)/mmd/BiasedFull.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/BiasedFull.cpp $(INCLUDES) -o $(INTERNALS)/mmd/BiasedFull.o
$(INTERNALS)/mmd/UnbiasedFull.o: $(INTERNALS)/mmd/UnbiasedFull.h $(INTERNALS)/mmd/UnbiasedFull.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/UnbiasedFull.cpp $(INCLUDES) -o $(INTERNALS)/mmd/UnbiasedFull.o
$(INTERNALS)/mmd/UnbiasedIncomplete.o: $(INTERNALS)/mmd/UnbiasedIncomplete.h $(INTERNALS)/mmd/UnbiasedIncomplete.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/UnbiasedIncomplete.cpp $(INCLUDES) -o $(INTERNALS)/mmd/UnbiasedIncomplete.o
$(INTERNALS)/mmd/FullDirect.o: $(INTERNALS)/mmd/FullDirect.h $(INTERNALS)/mmd/FullDirect.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/FullDirect.cpp $(INCLUDES) -o $(INTERNALS)/mmd/FullDirect.o
$(INTERNALS)/mmd/WithinBlockDirect.o: $(INTERNALS)/mmd/WithinBlockDirect.h $(INTERNALS)/mmd/WithinBlockDirect.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/WithinBlockDirect.cpp $(INCLUDES) -o $(INTERNALS)/mmd/WithinBlockDirect.o
$(INTERNALS)/mmd/WithinBlockPermutation.o: $(INTERNALS)/mmd/WithinBlockPermutation.h $(INTERNALS)/mmd/WithinBlockPermutation.cpp
	$(CC) $(OPTS) -c $(INTERNALS)/mmd/WithinBlockPermutation.cpp $(INCLUDES) -o $(INTERNALS)/mmd/WithinBlockPermutation.o
clean:
	rm libflash.so
