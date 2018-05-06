c compile:
	@CC=/usr/bin/gcc-8 CXX=/usr/bin/g++-8 cmake -H. -Bbuild
	@CC=/usr/bin/gcc-8 CXX=/usr/bin/g++-8 cmake --build build -- -j3

run:
	@./build/api
