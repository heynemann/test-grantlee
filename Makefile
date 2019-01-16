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
	@cd build && bash -c "cmake -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release .. && make"

clean:
	@rm -rf ./build

run: docker-build
	@docker run -it -v"`pwd`:/app" -p"8888:8888" jinja2cpp /bin/bash -c "make compile && ./build/api -vvv --templates /app/templates/"

run-prod: docker-build
	@docker run -it -v"`pwd`:/app" -p"8888:8888" jinja2cpp /bin/bash -c "make compile && ./build/api --templates /app/templates/"

docker-build:
	@docker build -t jinja2cpp .

docker-run:
	@docker run -it -v"`pwd`:/app" jinja2cpp 

bench:
	@wrk -c30 -d 30 -t10 --latency -s post.lua "http://localhost:8888/render?templ=alone.html"
