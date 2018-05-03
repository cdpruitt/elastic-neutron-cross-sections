SHELL = /bin/sh
BIN = bin/
COMPILER= clang++ 
CFLAGS= -lstdc++ -I$(shell root-config --incdir) -std=c++11 -ggdb
SOURCES = src/dataPoint.cpp src/dataSet.cpp 
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

LINKOPTION = $(shell root-config --libs)

TARGETS = readDataToGraph scaleCSDataFile calculateCS histos subtractBackground
all: $(addprefix $(BIN), $(TARGETS))

# produce gated histograms from raw ROOT trees with event data
$(BIN)histos: src/histos.cpp src/Detector.cpp src/PIDBanana.cpp
	$(COMPILER) $(CFLAGS) -o $(BIN)histos src/histos.cpp src/Detector.cpp src/PIDBanana.cpp $(LINKOPTION)

# using gated histograms for each run, produce difference histograms between
# target and background for each angle and detector
$(BIN)subtractBackground: src/subtractBackground.cpp src/ScalerData.cpp src/RunConfig.cpp 
	$(COMPILER) $(CFLAGS) -o $(BIN)subtractBackground src/subtractBackground.cpp src/ScalerData.cpp src/RunConfig.cpp $(LINKOPTION)

# using difference histograms and a reference cross section, produce cross
# sections by angle, target, and detector
$(BIN)calculateCS: src/calculateCS.cpp src/ScalerData.cpp src/RunConfig.cpp
	$(COMPILER) $(CFLAGS) -o $(BIN)calculateCS src/calculateCS.cpp src/ScalerData.cpp src/RunConfig.cpp $(LINKOPTION)

# read data points from text files to ROOT graphs
$(BIN)readDataToGraph: src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o $(BIN)readDataToGraph src/readDataToGraph.cpp src/dataSet.cpp src/dataPoint.cpp $(LINKOPTION)

$(BIN)scaleCSDataFile: src/scaleCSDataFile.cpp src/dataSet.cpp src/dataPoint.cpp
	$(COMPILER) $(CFLAGS) -o $(BIN)scaleCSDataFile src/scaleCSDataFile.cpp $(LINKOPTION)

%.o : %.cpp $(HEADERS)
	$(COMPILER) $(CFLAGS) $< -o $@

#.PHONY: all

clean:
	rm -f *.o
