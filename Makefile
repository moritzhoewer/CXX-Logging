INCLUDE_DIR = include
OUTPUT_FILE = logging-example.out

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pedantic -Wextra
INCLFLAGS = -I $(INCLUDE_DIR) 

SOURCES += $(wildcard src/logging/*.cpp)
SOURCES += main.cpp

all: $(SOURCES)
	$(CXX) -o $(OUTPUT_FILE) $(CXXFLAGS) $(INCLFLAGS) $(SOURCES)
	
.PHONY: clean
clean:
	rm -f $(OUTPUT_FILE)
	rm -f output.log