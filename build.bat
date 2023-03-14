set path=%path%;"C:\Program Files (x86)\CMake\bin\"
dir protobuf3 | find "cmake"   
if %errorlevel%==0 goto found
if %errorlevel%==1 goto not_found

:not_found
echo "无法找到protobuf3子目录，windows下需要手工解压protobuf-all-3.5.1.tar.gz到protobuf3子目录"
goto end

:found
cd protobuf3\cmake\
dir | find "build"   
if %errorlevel%==0 goto makefile_found
if %errorlevel%==1 goto makefile_not_found

:makefile_not_found
del /S /Q /F build
rd /S /Q build
md build
cd build
cmake.exe -G "MinGW Makefiles" -DCMAKE_C_FLAGS="-Wno-narrowing" -DCMAKE_CXX_FLAGS="-Wno-narrowing" ..
cd ..

:makefile_found
cd build
mingw32-make.exe

cd ..\..\..
del /S /Q /F build >nul
rd /S /Q build
md build
cd build
cmake.exe -G "MinGW Makefiles" ..
mingw32-make.exe
del ..\bin\test_codec.exe
move /y pbstru.exe ..\bin

cd ..\bin
call gen_codec.bat
cd ..\test
del /S /Q /F build
rd /S /Q build
md build
cd build
cmake.exe -G "MinGW Makefiles" ..
mingw32-make.exe
del ..\..\bin\test_codec.exe
move /y test_codec.exe ..\..\bin
cd ..\..\bin
test_codec.exe
cd ..

:end
echo 'Done."

