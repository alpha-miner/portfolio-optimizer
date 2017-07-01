#!/bin/sh

cd OpenBLAS

make -j8
make install PREFIX=$PWD

cd ../Ipopt
./configure PREFIX=$PWD
make -j8
make install

cd ../Eigen
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make install
