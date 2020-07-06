@echo off

set path=%path%;"C:\Program Files\CMake\bin";"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin"
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
cmake.exe -DCMAKE_C_FLAGS="-Wno-narrowing" -DCMAKE_CXX_FLAGS="-Wno-narrowing" ..
cd ..

:makefile_found
cd build
msbuild.exe protoc.vcxproj
copy Debug\protoc.exe ..\..\..\bin
cd ..\..\..
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
cmake.exe ..
msbuild.exe pbstru.vcxproj
move Debug\pbstru.exe ..\bin
cd ..\bin
call gen_codec.bat
cd ..\test
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
cmake.exe ..
msbuild.exe test_codec.vcxproj
move /y Debug\test_codec.exe ..\..\bin
cd ..\..\bin
test_codec.exe
cd ..

:end
echo 'Done."

