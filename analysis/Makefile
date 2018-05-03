SHELL = /bin/sh
COMPILER= clang++ 
CFLAGS= -lstdc++ -I$(shell root-config --incdir) -std=c++11 -ggdb
SOURCES = src/dataPoint.cpp src/dataSet.cpp 
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

LINKOPTION = $(shell root-config --libs)

all: readDataToGraph scaleCSDataFile calculateCS histos subtractBackground

readDataToGraph: src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o readDataToGraph src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp $(LINKOPTION)

scaleCSDataFile: src/scaleCSDataFile.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o scaleCSDataFile src/scaleCSDataFile.cpp $(LINKOPTION)

subtractBackground: src/subtractBackground.cpp src/ScalerData.cpp src/RunConfig.cpp 
	$(COMPILER) $(CFLAGS) -o subtractBackground src/subtractBackground.cpp src/ScalerData.cpp src/RunConfig.cpp $(LINKOPTION)

calculateCS: src/calculateCS.cpp src/ScalerData.cpp src/RunConfig.cpp src/DiffGroup.cpp
	$(COMPILER) $(CFLAGS) -o calculateCS src/calculateCS.cpp src/ScalerData.cpp src/RunConfig.cpp src/DiffGroup.cpp $(LINKOPTION)

histos: src/histos.cpp src/Detector.cpp src/PIDBanana.cpp
	$(COMPILER) $(CFLAGS) -o histos src/histos.cpp src/Detector.cpp src/PIDBanana.cpp $(LINKOPTION)

roughCS: src/roughCS.cpp
	$(COMPILER) $(CFLAGS) -o roughCS src/roughCS.cpp $(LINKOPTION)

%.o : %.cpp $(HEADERS)
	$(COMPILER) $(CFLAGS) $< -o $@

#.PHONY: all

clean:
	rm -f *.o
