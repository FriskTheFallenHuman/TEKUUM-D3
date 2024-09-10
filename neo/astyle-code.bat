@echo off
cls

astyle.exe -v --formatted --options=astyle-options.ini --exclude="libs" --recursive *.h
astyle.exe -v --formatted --options=astyle-options.ini --exclude="libs" --exclude="game/gamesys/SysCvar.cpp" --exclude="game/gamesys/Callbacks.cpp" --exclude="sys/win32/win_cpu.cpp" --recursive *.cpp

@pause