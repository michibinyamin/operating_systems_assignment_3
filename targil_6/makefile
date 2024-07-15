CXX = g++
CXXFLAGS = -Wall -g
TARGET = main
SRC = main.cpp Graph.cpp Reactor.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)
