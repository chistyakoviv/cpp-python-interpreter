py:
	python3 test.py

.PHONY: build

build:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	cmake --build .
