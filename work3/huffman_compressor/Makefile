CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = huffman_compressor
SOURCES = main.cpp FileCompressor.cpp HuffmanTree.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = FileCompressor.h HuffmanTree.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f *.exe

