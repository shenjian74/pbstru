pushd ..\protobuf3\cmake
call build.bat
popd

del /S /Q /F build
rd /S /Q build
md build
cd build
cmake -G "CodeBlocks - MinGW Makefiles" ..
mingw32-make
move pbstru.exe ../bin