FROM ubuntu:16.04

RUN apt-get update && apt-get upgrade -y && apt-get clean
# bzip2  libc6-dev-i386 g++-multilib openjdk-7-jdk

RUN apt-get install -y dialog \
	gcc g++ \
	cmake make \
	python \
	curl \
	software-properties-common \
	libboost-dev \
	libboost-system-dev \
	libboost-thread-dev \
	libboost-filesystem-dev \
	libboost-program-options-dev \
	libboost-python-dev \
	libboost-regex-dev \
	libmagick++-dev \
	libfreetype6-dev \
	libgl1-mesa-dev libglu1-mesa-dev \
	libgmp3-dev libmpfr-dev \
	libeigen3-dev \
	bison flex

# Ubuntu 14.04 comes with V8 engine version 3.14.5.8. This version is too old.
# Version 6.6 or higher is required. The ppa 'ppa:pinepain/libv8' includes a
# suitable version.

RUN add-apt-repository ppa:pinepain/libv8
RUN apt-get update && apt-get upgrade
RUN apt-get install -y libv8-6.6 libv8-6.6-dev
ENV V8_VER "6.6"
# RUN apt-get install -y libv8-dev

# RUN apt-get install -y libcgal-dev
RUN mkdir /usr/src/cgal
RUN curl -SL http://github.com/CGAL/cgal/archive/master.tar.gz | tar -xzC /usr/src/cgal
WORKDIR /usr/src/cgal
RUN cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_CXX_FLAGS=-frounding-math -Wall \
          -DWITH_GMP=true \
          -DWITH_CGAL_Qt5=OFF \
          cgal-master
RUN make install
