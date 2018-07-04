@echo off
call Windows-Clean.bat
mkdir Build
cd Build
cmake -G "Visual Studio 15 2017 Win64" ..
mkdir Debug
mkdir Release
copy ..\Engine\Dependencies\bin\Debug\x64\ Debug
copy ..\Engine\Dependencies\bin\Release\x64\ Release

cd ..