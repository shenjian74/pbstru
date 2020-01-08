@echo off

dir protobuf3 | find "cmake"   
if %errorlevel%==0 goto found
if %errorlevel%==1 goto not_found

:not_found
echo "Cannot find protbuf3."
goto end

:found
cd protobuf3\cmake\
dir | find "build"   
if %errorlevel%==0 goto makefile_found
if %errorlevel%==1 goto makefile_not_found

:makefile_not_found
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
"C:\Program Files\CMake\bin\cmake.exe" -G "CodeBlocks - MinGW Makefiles" -DCMAKE_C_FLAGS="-Wno-narrowing" -DCMAKE_CXX_FLAGS="-Wno-narrowing" ..
cd ..

:makefile_found
cd build
mingw32-make
copy protoc.exe ..\..\..\bin
cd ..\..\..
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
"C:\Program Files\CMake\bin\cmake.exe" -G "CodeBlocks - MinGW Makefiles" ..
mingw32-make
move pbstru.exe ..\bin\
cd ..\bin
call gen_codec.bat
cd ..\test
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
"C:\Program Files\CMake\bin\cmake.exe" -G "CodeBlocks - MinGW Makefiles" ..
mingw32-make
test_codec.exe
cd ..\..

:end
echo 'Done."

