SHELL = /bin/sh
BIN = bin/
COMPILER= clang++ 
CFLAGS= -lstdc++ -I$(shell root-config --incdir) -std=c++11 -ggdb
SOURCES = src/dataPoint.cpp src/dataSet.cpp 
HEADERS = $(shell echo include/*.h)
OBJECTS = $(SOURCES:.c=.o)

LINKOPTION = $(shell root-config --libs)

TARGETS = readDataToGraph scaleCSDataFile driver
all: $(addprefix $(BIN), $(TARGETS))

# run analysis in three steps:
# 	- produce gated histograms from raw ROOT trees with event data
# 	
# 	- using gated histograms for each run, produce difference histograms between
# 	  target and blank for each angle
#
# 	- using difference histograms and a reference cross section, produce cross
# 	  sections by angle, target, and detector
$(BIN)driver: src/driver.cpp src/histos.cpp src/subtractBackground.cpp src/calculateCS.cpp src/Detector.cpp src/PIDBanana.cpp src/Config.cpp
	$(COMPILER) $(CFLAGS) -o $(BIN)driver src/driver.cpp src/histos.cpp src/subtractBackground.cpp src/calculateCS.cpp src/Detector.cpp src/PIDBanana.cpp\
	    src/Config.cpp $(LINKOPTION)

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
