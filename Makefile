CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g

TARGET := eight_puzzle
SOURCES := ./src/main.cpp ./src/Puzzle.cpp ./src/A_Star.cpp
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJECTS) $(TARGET)