@echo off

set BUILD_TYPE=RELEASE

cd portfolio-optimizer

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..

cd ../..

@echo on