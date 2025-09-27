# Default target
all: build/tinydb

# Configure + build with CMake
build/tinydb:
	mkdir -p build
	cd build && cmake .. && make

# Run the binary
run: all
	./build/tinydb

# Clean build artifacts
clean:
	rm -rf build
