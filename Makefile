build:
	mkdir -p build
	cd build && cmake .. -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

clean:
	rm -rf build
