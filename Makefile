SOURCE_CODE=src/*.cpp
MAIN=cpp_sample.cc
CPP_VERSION=c++14

build:
	g++ --std=$(CPP_VERSION) $(MAIN) $(SOURCE_CODE) -O2 -o linda.out

debug:
	g++ -g --std=$(CPP_VERSION) $(MAIN) $(SOURCE_CODE) -o linda.out
