# Install static library at first.
# -- sudo yum install libstdc++-static glibc-static
rm -rf build
mkdir build
cd build
cmake ..
make
mv pbstru ../bin
