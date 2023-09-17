lithium = ../..

.PHONY: testassetloading
testassetloading:
	g++ -std=c++17 -o testassetloading testassetloading.cpp $(lithium)/src/glfilesystem.cpp -I $(lithium)/include/lithium/utility -lstdc++fs -lpthread \
		&& ./testassetloading