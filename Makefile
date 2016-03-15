OPTS	 = -O3 -std=c++14
INCLUDES = -Isrc
LIBS	 = -Lsrc -lflash -lshogun
TESTS    = tests
all: $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest $(TESTS)/InitPerFeatureTest \
	 $(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/KernelManagerTest \
	 $(TESTS)/MMDTest
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
$(TESTS)/KernelManagerTest: $(TESTS)/KernelManagerTest.cpp src/libflash.so
	g++ $(TESTS)/KernelManagerTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/KernelManagerTest
$(TESTS)/MMDTest: $(TESTS)/MMDTest.cpp src/libflash.so
	g++ $(TESTS)/MMDTest.cpp $(OPTS) $(INCLUDES) $(LIBS) -o $(TESTS)/MMDTest
clean:
	rm $(TESTS)/DataFetcherTest $(TESTS)/StreamingDataFetcherTest $(TESTS)/InitPerFeatureTest \
	   $(TESTS)/DataFetcherFactoryTest $(TESTS)/DataManagerTest $(TESTS)/KernelManagerTest \
	   $(TESTS)/MMDTest
