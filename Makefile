CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -march=native
TARGET = matrix_profiling
SOURCE = matrix_profiling.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean 