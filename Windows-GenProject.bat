@echo off
call Windows-Clean.bat
mkdir Build
cd Build
cmake -G "Visual Studio 15 2017 Win64" ..
mkdir Debug
mkdir Release
copy ..Engine\Dependencies\bin\ Debug
copy ..Engine\Dependencies\bin\ Release

cd ..