CXX = g++
CXXFLAGS = -std=c++14 -Wall
TARGET = decoder
SRCS = src/main.cpp src/elf_reader.cpp src/riscv_decoder.cpp

# For testing
TEST_TARGET = run_tests
TEST_SRCS = tests/test_decoder.cpp src/riscv_decoder.cpp src/elf_reader.cpp

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# requires libgtest-dev
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET):
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o $(TEST_TARGET) -lgtest -lgtest_main -pthread

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.exe
