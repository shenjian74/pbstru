del /s /q /f build
rd /s /q build
md build
cd build
cmake -G "CodeBlocks - MinGW Makefiles" ..
mingw32-make
move pbstru.exe ../bin