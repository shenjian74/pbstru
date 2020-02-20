rm -rf build
mkdir build
cd build
cmake ..
make
cp test_codec ../../bin
cd ../../bin
./test_codec
