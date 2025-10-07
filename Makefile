CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iheaders -Itemplates
LDFLAGS := -lgtest -lgtest_main -pthread

# Collect all cpp files automatically
SRC := main.cpp $(wildcard sources/*.cpp)

# Object files
OBJ := $(patsubst %.cpp, build/%.o, $(SRC))

# Executable
TARGET := build/debug/main

.PHONY: all clean run dirs

all: dirs $(TARGET)

dirs:
	mkdir -p build build/debug build/sources

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build
