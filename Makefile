CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -Weffc++

SOURCES= $(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
TARGET=$(lastword $(subst /, ,$(realpath .)))

CXX=clang++

all: $(TARGET)
	@echo evolve_l built

$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) -o evolve_l $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(TARGET)
