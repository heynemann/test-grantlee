FROM ubuntu:18.10

RUN apt-get update -y && apt-get install -y libboost-all-dev
RUN apt-get install -y cmake bash wget software-properties-common build-essential
#RUN apt-get install -y clang
#RUN add-apt-repository ppa:ubuntu-toolchain-r/test
#RUN apt update -y
RUN apt-get install gcc-7 g++-7 -y
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 \
                         --slave /usr/bin/g++ g++ /usr/bin/g++-7 
RUN update-alternatives --config gcc
RUN gcc --version
RUN g++ --version

#RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
#RUN apt-add-repository -y "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
#RUN apt-get update -y && apt-get install -y clang-5.0

# Installing Jinja2Cpp
RUN mkdir -p /usr/local/
RUN apt-get install -y git
RUN mkdir /jinja2cpp
RUN git clone https://github.com/flexferrum/Jinja2Cpp.git /jinja2cpp
WORKDIR /jinja2cpp
RUN git checkout 0.9.1
RUN git submodule -q update --init
ENV CC gcc
ENV CXX g++
RUN mkdir build && cd build && \
        cmake .. -DCMAKE_INSTALL_PREFIX=/jinja2cpp && \
        cmake --build . --target all && \
        cmake --build . --target install && \
        ctest -C release

RUN cp -r /jinja2cpp/include/* /usr/include
RUN cp -r /jinja2cpp/lib/* /usr/lib
# Jinja2Cpp installed

RUN apt-get autoremove -y && apt-get clean -y

RUN mkdir -p /app
COPY . /app
WORKDIR /app

ENV JINJA2CPP_INSTALL_DIR /jinja2cpp 
