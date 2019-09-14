#!/bin/sh

# Add current build time, has move to pre-commit script.
# current_time=`date +"%Y-%m-%d %H:%M:%S"`
# sed s/MYVERSION/"build: $current_time"/g pbstru/version.h.tmp > pbstru/version.h

# Install static library at first.
# -- sudo yum install libstdc++-static glibc-static

if [ ! -f "protobuf3/Makefile" ]; then 
  # wget --timestamping https://github.com/protocolbuffers/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz && \
  tar xvfz protobuf-all-3.5.1.tar.gz && \
  rm -rf protobuf3 && \
  mv protobuf-3.5.1 protobuf3 && \
  cd protobuf3 && \
  ./autogen.sh && \
  ./configure --disable-shared && \
  cd ..
fi

if [ -f "protobuf3/Makefile" ]; then 
  cd protobuf3
  make
  cd ..
  rm -rf build && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make && \
  mv pbstru ../bin && \
  cd ../bin && \
  rm -rf codec &&\
  ./pbstru cudr cdb.proto cdb_ccc.proto codec && \
  mkdir ../build/test && \
  cd ../build/test && \
  cmake ../../test && \
  make && \
  ./test_codec
fi
