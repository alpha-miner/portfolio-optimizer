@echo off

set BUILD_TYPE=Release
set BOOST_ROOT=d:\dev\boost_1_64_0

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