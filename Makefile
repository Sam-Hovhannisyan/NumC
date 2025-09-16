CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iheaders -Itemplates
LDFLAGS := -lgtest -lgtest_main -pthread

# Only compile main and test sources (templates/numc.cpp is included in headers)
SRC := main.cpp
UTEST_SRC := main_utest.cpp

# Object files in build/
OBJ := $(patsubst %.cpp, build/%.o, $(SRC))
UTEST_OBJ := $(patsubst %.cpp, build/%.o, $(UTEST_SRC))

# Executables in build/debug/
TARGET := build/debug/main
UTEST_TARGET := build/debug/main_utest

.PHONY: all clean run test dirs

all: dirs $(TARGET) $(UTEST_TARGET)

# Ensure build directories exist
dirs:
	mkdir -p build build/debug

# Link main program
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Link unit tests
$(UTEST_TARGET): $(UTEST_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile .cpp -> .o
build/%.o: %.cpp headers/numc.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run unit tests
test: $(UTEST_TARGET)
	./$(UTEST_TARGET)

# Clean everything
clean:
	rm -rf build
