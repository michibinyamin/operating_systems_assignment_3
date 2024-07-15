# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -pthread

# Executable name
TARGET = main

# Source files
SOURCES = main.cpp Graph.cpp Proactor.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Build target
all: $(TARGET)

# Link the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Phony targets
.PHONY: all clean
