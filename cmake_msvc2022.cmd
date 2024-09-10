@echo off
if exist build\msvc2022 (rmdir build\msvc2022 /s /q)
mkdir build\msvc2022
pushd build\msvc2022
cmake -G "Visual Studio 17" %* ../../neo
popd
@pause