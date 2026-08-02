CXX = c++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Wpedantic

build/orderbook: src/main.cpp src/orderbook.cpp src/main.h
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/main.cpp src/orderbook.cpp -o build/orderbook

.PHONY: clean
clean:
	rm -rf build
