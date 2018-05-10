build c compile:
	@mkdir -p build
	@cd build && bash -c "CC=/usr/bin/clang-6.0 CXX=/usr/bin/clang++-6.0 cmake .. -DCMAKE_BUILD_TYPE=Release && make"
	#@CC=/usr/bin/gcc-8 CXX=/usr/bin/g++-8 cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
	#@CC=/usr/bin/gcc-8 CXX=/usr/bin/g++-8 cd build && make

clean:
	@rm -rf ./build

run:
	@./build/api 1000
	#@cutelyst-wsgi2 \
		#--http-socket :8080 \
		#--application ./build/libMyApp.so \
		#--processes=8 \
		#--threads=16 \
		#--cpu-affinity=1
