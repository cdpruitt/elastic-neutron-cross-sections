SHELL = /bin/sh
COMPILER= clang++ 
CFLAGS= -lstdc++ -I$(shell root-config --incdir) -std=c++11 -ggdb
SOURCES = src/dataPoint.cpp src/dataSet.cpp 
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

LINKOPTION = $(shell root-config --libs)

all: readDataToGraph scaleCSDataFile calculateCS histos 

readDataToGraph: src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o readDataToGraph src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp $(LINKOPTION)

scaleCSDataFile: src/scaleCSDataFile.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o scaleCSDataFile src/scaleCSDataFile.cpp $(LINKOPTION)

calculateCS: src/calculateCS.cpp src/ScalerData.cpp
	$(COMPILER) $(CFLAGS) -o calculateCS src/calculateCS.cpp src/ScalerData.cpp $(LINKOPTION)

histos: src/histos.cpp src/Detector.cpp
	$(COMPILER) $(CFLAGS) -o histos src/histos.cpp src/Detector.cpp $(LINKOPTION)

%.o : %.cpp $(HEADERS)
	$(COMPILER) $(CFLAGS) $< -o $@

#.PHONY: all

clean:
	rm -f *.o
