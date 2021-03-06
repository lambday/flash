CC		 = ccache g++
OPTS	 = -g3 -std=c++14 -fopenmp
INCLUDES = -Isrc
LIBS	 = -Lsrc -lflash -lshogun
TESTS    = tests
all: $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest $(TESTS)/InitPerFeatureTest \
	 $(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/KernelManagerTest \
	 $(TESTS)/MMDTest
$(TESTS)/DataFetcherTest: $(TESTS)/DataFetcherTest.cpp src/libflash.so
	$(CC) $(TESTS)/DataFetcherTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataFetcherTest
$(TESTS)/StreamingDataFetcherTest: $(TESTS)/StreamingDataFetcherTest.cpp src/libflash.so
	$(CC) $(TESTS)/StreamingDataFetcherTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/StreamingDataFetcherTest
$(TESTS)/DataFetcherFactoryTest: $(TESTS)/DataFetcherFactoryTest.cpp src/libflash.so
	$(CC) $(TESTS)/DataFetcherFactoryTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataFetcherFactoryTest
$(TESTS)/InitPerFeatureTest: $(TESTS)/InitPerFeatureTest.cpp src/libflash.so
	$(CC) $(TESTS)/InitPerFeatureTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/InitPerFeatureTest
$(TESTS)/DataManagerTest: $(TESTS)/DataManagerTest.cpp src/libflash.so
	$(CC) $(TESTS)/DataManagerTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataManagerTest
$(TESTS)/KernelManagerTest: $(TESTS)/KernelManagerTest.cpp src/libflash.so
	$(CC) $(TESTS)/KernelManagerTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/KernelManagerTest
$(TESTS)/MMDTest: $(TESTS)/MMDTest.cpp src/libflash.so
	$(CC) $(TESTS)/MMDTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/MMDTest
clean:
	rm $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest $(TESTS)/InitPerFeatureTest \
	   $(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/KernelManagerTest \
	   $(TESTS)/MMDTest
