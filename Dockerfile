# Start with a stable version of Ubuntu, namely ubuntu:14.04
# Version 14.04 comes with CGAL version 4,2, which is a bit old, but for
# now will do.
# Version 15:04, while comes with a more recent version of CGAL, namely 4.5,
# is not stable. Some problems occur when compiling sources that depend on
# ImageMagick.

FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y cmake
RUN apt-get install -y libboost-dev
RUN apt-get install -y libboost-system-dev
RUN apt-get install -y libboost-filesystem-dev
RUN apt-get install -y libboost-program-options-dev
RUN apt-get install -y libmagick++-dev
RUN apt-get install -y libgl1-mesa-dev
RUN apt-get install -y libcgal-dev
RUN apt-get install -y bison flex
RUN apt-get install -y g++
RUN apt-get install -y libglu1-mesa-dev

RUN mkdir /sgal
WORKDIR /sgal
COPY ./CMakeLists.txt ./
ADD ./src ./src
ADD ./cmake ./cmake

RUN cmake -DSGAL_USE_POS:BOOL=OFF \
          -DSGAL_USE_VOS:BOOL=OFF \
	  -DSGAL_USE_CGM:BOOL=OFF \
	  -DSGAL_USE_LTS:BOOL=OFF \
	  -DSGAL_USE_SGM:BOOL=OFF \
          -DCGAL_CXX_FLAGS:STRING="-DSGAL_TRACE" .
RUN make
