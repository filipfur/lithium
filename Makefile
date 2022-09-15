

target:
	mkdir -p cmake-build
	cd cmake-build && \
	cmake ..

clean:
	rm -rf cmake-build
