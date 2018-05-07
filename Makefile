c compile:
	@CC=`which gcc-8` CXX=`which g++-8` cmake -H. -Bbuild
	@CC=`which gcc-8` CXX=`which g++-8` cmake --build build -- -j3

run:
	@./build/api
