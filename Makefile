OPTS	 = -O3 -std=c++14
INCLUDES = -Isrc -I/usr/include/eigen3
LIBS	 = -Lsrc -lflash -lshogun
TESTS    = tests
all: $(TESTS)/NextSamplesTest $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest \
		$(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/InitPerFeatureTest
$(TESTS)/NextSamplesTest: $(TESTS)/NextSamplesTest.cpp src/libflash.so
	g++ $(TESTS)/NextSamplesTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/NextSamplesTest
$(TESTS)/DataFetcherTest: $(TESTS)/DataFetcherTest.cpp src/libflash.so
	g++ $(TESTS)/DataFetcherTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataFetcherTest
$(TESTS)/StreamingDataFetcherTest: $(TESTS)/StreamingDataFetcherTest.cpp src/libflash.so
	g++ $(TESTS)/StreamingDataFetcherTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/StreamingDataFetcherTest
$(TESTS)/DataFetcherFactoryTest: $(TESTS)/DataFetcherFactoryTest.cpp src/libflash.so
	g++ $(TESTS)/DataFetcherFactoryTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataFetcherFactoryTest
$(TESTS)/InitPerFeatureTest: $(TESTS)/InitPerFeatureTest.cpp src/libflash.so
	g++ $(TESTS)/InitPerFeatureTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/InitPerFeatureTest
$(TESTS)/DataManagerTest: $(TESTS)/DataManagerTest.cpp src/libflash.so
	g++ $(TESTS)/DataManagerTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/DataManagerTest
clean:
	rm $(TESTS)/NextSamplesTest $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest \
	   $(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/InitPerFeatureTest
