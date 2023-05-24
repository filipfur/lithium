lithium = ../..

.PHONY: testfilewatch
testfilewatch:
	g++ -std=c++17 -o testfilewatch testfilewatch.cpp $(lithium)/src/glfilewatch.cpp -I $(lithium)/include/lithium/utility -lstdc++fs -lpthread \
		&& ./testfilewatch