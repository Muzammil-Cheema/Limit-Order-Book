# C++ 26 compiler path
CXX_PATH = "/opt/homebrew/opt/llvm/bin/clang++"

.PHONY: all build run clean

all: build
	CXX=${CXX_PATH} cmake --build build

build: 
	CXX=${CXX_PATH} cmake -B build

run: all
	./bin/matching_engine

clean:
	rm -rf build bin