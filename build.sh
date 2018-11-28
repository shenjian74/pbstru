#!/bin/sh

# Add current build time, has move to pre-commit script.
# current_time=`date +"%Y-%m-%d %H:%M:%S"`
# sed s/MYVERSION/"build: $current_time"/g pbstru/version.h.tmp > pbstru/version.h

# Install static library at first.
# -- sudo yum install libstdc++-static glibc-static
rm -rf build
mkdir build
cd build
cmake ..
make
mv pbstru ../bin
