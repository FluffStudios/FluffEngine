@echo off
call Windows-Clean.bat
mkdir Build
cd Build
cmake ../ -DCMAKE_GENERATOR_PLATFORM=x64
mkdir Debug
mkdir Release
copy ..Engine\Dependencies\bin\ Debug
copy ..Engine\Dependencies\bin\ Release

cd ..