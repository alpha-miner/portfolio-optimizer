@echo off

set BUILD_TYPE=Release
set BOOST_ROOT=d:\dev\boost_1_64_0

cd alglib

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..

msbuild alglib.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=x64

cd ../../portfolio-optimizer

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..

msbuild pfopt.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=x64

cd ../..

@echo on