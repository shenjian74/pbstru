#!/bin/sh

# Add current build time, has move to pre-commit script.
# current_time=`date +"%Y-%m-%d %H:%M:%S"`
# sed s/MYVERSION/"build: $current_time"/g pbstru/version.h.tmp > pbstru/version.h

# Install static library at first.
# -- sudo yum install libstdc++-static glibc-static

if [ ! -d "protobuf3" ]; then 
  wget https://github.com/protocolbuffers/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz && tar xvfz protobuf-all-3.5.1.tar.gz && mv protobuf-3.5.1 protobuf3
fi

rm -rf build && mkdir build && cd build && cmake .. && make && mv pbstru ../bin
