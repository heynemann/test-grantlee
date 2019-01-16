FROM ubuntu:18.10

RUN apt-get update -y && apt-get install -y libboost-all-dev
RUN apt-get install -y cmake bash wget software-properties-common build-essential
RUN apt-get install -y clang

#RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
#RUN apt-add-repository -y "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main"
#RUN apt-get update -y && apt-get install -y clang-5.0

# Installing injaJinja2Cpp
RUN cd /tmp && wget https://github.com/pantor/inja/archive/v2.0.1.tar.gz && tar xvzf v2.0.1.tar.gz
RUN cp -r /tmp/inja-2.0.1/single_include/inja/inja.hpp /usr/include
RUN cp -r /tmp/inja-2.0.1/include/nlohmann /usr/include
#RUN mkdir -p /usr/local/
#RUN apt-get install -y git
#RUN mkdir /jinja2cpp
#RUN git clone https://github.com/flexferrum/Jinja2Cpp.git /jinja2cpp
#WORKDIR /jinja2cpp
#RUN git submodule -q update --init
#ENV CC clang
#ENV CXX clang++
#RUN mkdir build && cd build && \
        #cmake .. -DCMAKE_INSTALL_PREFIX=/jinja2cpp && \
        #cmake --build . --target all && \
        #cmake --build . --target install && \
        #ctest -C release
# Jinja2Cpp installed

RUN apt-get autoremove -y && apt-get clean -y

RUN mkdir -p /app
COPY . /app
WORKDIR /app

ENV JINJA2CPP_INSTALL_DIR /jinja2cpp 
