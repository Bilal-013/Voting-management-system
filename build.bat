@echo off
REM Build script for Voting-management-system using g++ (MinGW)
cd src

g++ -std=c++11 -o ..\main.exe *.cpp

cd ..
echo Build complete. main.exe created.
