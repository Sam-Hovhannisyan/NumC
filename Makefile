CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iheaders -Itemplates
LDFLAGS := -lgtest -lgtest_main -pthread

# Only compile main and test sources (templates/Array.cpp is included in headers)
SRC := main.cpp

# Object files in build/
OBJ := $(patsubst %.cpp, build/%.o, $(SRC))

# Executables in build/debug/
TARGET := build/debug/main

.PHONY: all clean run test dirs

all: dirs $(TARGET)

# Ensure build directories exist
dirs:
	mkdir -p build build/debug

# Link main program
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp -> .o
build/%.o: %.cpp headers/Array.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean everything
clean:
	rm -rf build
