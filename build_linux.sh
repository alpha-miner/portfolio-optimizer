#!/bin/sh

export CXXFLAGS=-O3

cd OpenBLAS

make clean
make -j8
make install PREFIX=$PWD

cd ../Ipopt
./configure PREFIX=$PWD --with-blas="-L$PWD/../OpenBLAS/lib -lopenblas" --with-lapack="-L$PWD/../OpenBLAS/lib -lopenblas" ADD_CFLAGS=-fopenmp ADD_FFLAGS=-fopenmp ADD_CXXFLAGS=-fopenmp
make clean
make -j8
make install

cd ../eigen
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make clean
make install
