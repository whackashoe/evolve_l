CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++ -fopenmp 

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))

CXX=g++

all: $(TARGET)
	@echo evolve_l built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o evolve_l $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
