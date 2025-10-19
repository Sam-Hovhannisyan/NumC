CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iheaders -Itemplates
LDFLAGS := -lgtest -lgtest_main -pthread

# Only compile main program
SRC := main.cpp

# Object files
OBJ := $(patsubst %.cpp, build/%.o, $(SRC))

# Executable
TARGET := build/debug/main

.PHONY: all clean run dirs

all: dirs $(TARGET)

# Create build directories
dirs:
	mkdir -p build build/debug

# Compile main.cpp -> object
build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Run program
run: $(TARGET)
	./$(TARGET)

# Clean build
clean:
	rm -rf build
