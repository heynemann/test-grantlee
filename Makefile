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
	@cd build && bash -c "cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release .. && make"

clean:
	@rm -rf ./build

run: docker-build
	@docker run -it -v"`pwd`:/app" -p"8888:8888" jinja2cpp /bin/bash -c "make compile && ./build/api 6"

docker-build:
	@docker build -t jinja2cpp .

docker-run:
	@docker run -it -v"`pwd`:/app" jinja2cpp 
