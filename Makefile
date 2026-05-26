.PHONY: all build run clean

all: build
	cmake --build build

build: 
	cmake -B build

run: all
	./bin/engine

clean:
	rm -rf build bin