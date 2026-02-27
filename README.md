# RNGOEngine
OpenGL engine written in C++23 with a bundled editor developed using ImGui.

## Build Instructions
Prerequisites:
* CMake
* C++23 compatible toolchain.

This repo consists of two parts, the Runtime (compiled as a static library) and the Editor, compiled as an executable.

To generate a solution file, go into either the /Runtime or /Editor paths and use `cmake -G "GENERATOR_NAME"` replacing GENERATOR_NAME with your preferred [generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html). 
Example: `cd Runtime && cmake -G "Visual Studio 19"`

## Troubleshooting:
* Some generators and toolchains may not yet support C++23, try downloading a toolchain compatible with C++23 if it doesn't compile properly.
