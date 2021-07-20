# CMakeList.txt : CMake project for WahlBit, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("WahlBit")
# Add source to this project's executable.
add_executable (WahlBit "WahlBit.cpp" "WahlBit.h" "main.cpp")

# TODO: Add tests and install targets if needed.
