setup:
	@sudo apt-get install -y \
		build-essential \
		libtcmalloc-minimal4 \
		libboost-all-dev \
		libgrantlee5-dev \
		libgflags-dev		
	@sudo ln -sf /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

build c compile:
	@mkdir -p build
	@cd build && bash -c "CC=/usr/bin/clang-6.0 CXX=/usr/bin/clang++-6.0 cmake .. -DCMAKE_BUILD_TYPE=Release && make"

clean:
	@rm -rf ./build

run:
	@./build/api 6
