#!/bin/sh

export num_cores=`grep -c processor /proc/cpuinfo`

cd OpenBLAS

# make clean
make -j${num_cores}
make install PREFIX=$PWD

cd ../Ipopt

cd ThirdParty

chmod 777 -R ASL
chmod 777 -R Blas
chmod 777 -R Lapack
chmod 777 -R Mumps
chmod 777 -R Metis
chmod 777 -R HSL

cd ASL
./get.ASL

cd ../Blas
./get.Blas

cd ../Lapack
./get.Lapack

cd ../Mumps
./get.Mumps

cd ../Metis
./get.Metis

cd ../..

./configure --prefix=$PWD --with-blas="-L$PWD/../OpenBLAS/lib -lopenblas" --with-lapack="-L$PWD/../OpenBLAS/lib -lopenblas" ADD_CFLAGS=-fopenmp ADD_FFLAGS=-fopenmp ADD_CXXFLAGS=-fopenmp
make clean
make -j${num_cores}
make install

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../alglib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make clean
make -j${num_cores}

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../../clp
./configure --prefix=$PWD
make clean
make -j${num_cores}
make install

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../portfolio-optimizer
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. ..
make clean
make -j${num_cores}

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../bin
./test_suite

if [ $? -ne 0 ] ; then
    exit 1
fi

# build java interface for ipopt, please see README.md for detail

cd ../../Ipopt/

cd Ipopt/contrib/JavaInterface

export CLASSPATH=$PWD:$CLASSPATH
export CPLUS_INCLUDE_PATH=$JAVA_HOME/include/linux

make

if [ $? -ne 0 ] ; then
    exit 1
fi

echo "Portfolio - Optimizer building completed!"
