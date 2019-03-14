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
"C:\Program Files\CMake\bin\cmake.exe" -G "CodeBlocks - MinGW Makefiles" ..
cd ..

:makefile_found
cd build
mingw32-make
cd ..\..\..
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
"C:\Program Files\CMake\bin\cmake.exe" -G "CodeBlocks - MinGW Makefiles" ..
move pbstru.exe ..\bin\
cd ..\bin
pbstru.exe cdb.proto cdb_ccc.proto codec
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

