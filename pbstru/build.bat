del /S /Q /F build
rd /S /Q build
md build
cd build
cmake -G "CodeBlocks - MinGW Makefiles" ..
mingw32-make -j
move pbstru.exe ../bin