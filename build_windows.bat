@echo off

set BUILD_TYPE=Release
set GTEST_ROOT=d:\dev\googletest-release-1.8.0\googletest

cd alglib

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..

if %errorlevel% neq 0 exit /b 1

msbuild alglib.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=x64

if %errorlevel% neq 0 exit /b 1

cd ../../portfolio-optimizer

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build

if %BUILD_TEST% == ON (
    cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DTEST=ON ..
    if %errorlevel% neq 0 exit /b 1

    msbuild pfopt.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=x64
    if %errorlevel% neq 0 exit /b 1

    cd ../bin
    test_suite.exe
    if %errorlevel% neq 0 exit /b 1
    cd ../..
) else (
    echo Test Set is omitted
    cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
    if %errorlevel% neq 0 exit /b 1

    msbuild pfopt.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=x64
    if %errorlevel% neq 0 exit /b 1
    cd ../..
)

if exist include (
  rmdir /s /q include
  mkdir include
) else (
  mkdir include
)

md include\alglib
md include\clp
md include\ipopt
md include\eigen\Eigen
md include\pfopt

xcopy alglib\src include\alglib /Q
xcopy clp\windows\include include\clp /S /Q
xcopy Ipopt\windows\include include\ipopt /S /Q
xcopy eigen\Eigen include\eigen\Eigen /S /Q
xcopy portfolio-optimizer\pfopt include\pfopt /Q

if exist lib (
  rmdir /s /q lib
  mkdir lib
) else (
  mkdir lib
)

xcopy alglib\lib\*.lib lib /Q
xcopy clp\windows\lib\%BUILD_TYPE%\*.lib lib /Q
xcopy Ipopt\windows\lib\*.lib lib /Q
xcopy portfolio-optimizer\lib\*.lib lib /Q

@echo on