# Start with a stable version of Ubuntu, namely ubuntu:14.04
# Version 15:04 has some problems when compiling sources that depend on
# ImageMagick.
# Version 15.04 comes with CGAL version 4.5, which is too old anyway.

FROM ubuntu:14.04

RUN apt-get update && apt-get upgrade -y && apt-get clean
# bzip2  libc6-dev-i386 g++-multilib openjdk-7-jdk

RUN apt-get install -y dialog subversion gcc g++ make python curl software-properties-common

# Ubuntu 14.04 comes with V8 engine version 3.14.5.8. This version is too old.
# Version 4.9 or higher is required. The ppa 'ppa:pinepain/v8' includes a
# suitable version even though depricated. Other ppa's include more recent
# versions, but are targeted at vivid (ubuntu 15.04).

RUN add-apt-repository ppa:pinepain/v8
RUN apt-get update && apt-get upgrade
RUN apt-get install libv8-4.9.385 libv8-4.9-dev

RUN apt-get install -y cmake
RUN apt-get install -y g++
RUN apt-get install -y libboost-dev
RUN apt-get install -y libboost-system-dev
RUN apt-get install -y libboost-thread-dev
RUN apt-get install -y libboost-filesystem-dev
RUN apt-get install -y libboost-program-options-dev
RUN apt-get install -y libboost-python-dev
RUN apt-get install -y libmagick++-dev
RUN apt-get install -y libfreetype6-dev
# RUN apt-get install -y libv8-dev
RUN apt-get install -y libgl1-mesa-dev libglu1-mesa-dev
# RUN apt-get install -y libcgal-dev
RUN apt-get install -y libgmp3-dev libmpfr-dev
RUN apt-get install -y libeigen3-dev
RUN apt-get install -y bison flex

RUN mkdir /usr/src/cgal
RUN curl -SL http://github.com/CGAL/cgal/archive/master.tar.gz | tar -xzC /usr/src/cgal
WORKDIR /usr/src/cgal
RUN cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_CXX_FLAGS=-frounding-math -Wall \
          -DWITH_CGAL_PDB=OFF \
          -DWITH_GMP=true \
          -DWITH_CGAL_Qt3=OFF \
          -DWITH_CGAL_Qt5=OFF \
          cgal-master
RUN make install

RUN mkdir /usr/src/sgal
WORKDIR /usr/src/sgal
COPY ./CMakeLists.txt ./
ADD ./src ./src
ADD ./cmake ./cmake

RUN cmake -DCMAKE_BUILD_TYPE=Release \
          -DSGAL_USE_V8:BOOL=ON \
          -DSGAL_USE_SCGAL:BOOL=ON \
          -DSGAL_USE_POS:BOOL=OFF \
          -DSGAL_USE_VOS:BOOL=OFF \
          -DSGAL_USE_CGM:BOOL=OFF \
          -DSGAL_USE_LTS:BOOL=OFF \
          -DSGAL_USE_SGM:BOOL=OFF \
	  -DSGAL_USE_NGM:BOOL=OFF \
          -DSGAL_BUILD_PYBINDINGS:BOOL=ON \
          -DCGAL_CXX_FLAGS:STRING="-DSGAL_TRACE" .
RUN make install

# RUN apt-get install -y mesa-utils
