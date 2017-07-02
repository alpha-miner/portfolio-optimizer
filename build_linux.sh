#!/bin/sh

cd OpenBLAS

# make clean
make -j8
make install PREFIX=$PWD

cd ../Ipopt
./configure --prefix=$PWD --with-blas="-L$PWD/../OpenBLAS/lib -lopenblas" --with-lapack="-L$PWD/../OpenBLAS/lib -lopenblas" ADD_CFLAGS=-fopenmp ADD_FFLAGS=-fopenmp ADD_CXXFLAGS=-fopenmp
# make clean
make -j8
make install

cd ../eigen
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make uninstall
make -j8 install

cd ../../portfolio-optimizer
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make clean
make -j8

# build java interface for ipopt, please see README.md for detail

cd ../../Ipopt/

cd Ipopt/contrib/JavaInterface

export CLASSPATH=$PWD:$CLASSPATH

make