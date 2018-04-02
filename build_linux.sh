#!/bin/sh

export PING_SLEEP=30s
export WORKDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export BUILD_OUTPUT=$WORKDIR/build.out

touch $BUILD_OUTPUT

dump_output() {
   echo Tailing the last 500 lines of output:
   tail -500 $BUILD_OUTPUT
}
error_handler() {
  echo ERROR: An error was encountered with the build.
  dump_output
  exit 1
}
# If an error occurs, run our error handler to output a tail of the build
trap 'error_handler' ERR

# Set up a repeating loop to send some output to Travis.

bash -c "while true; do echo \$(date) - building ...; sleep $PING_SLEEP; done" &
PING_LOOP_PID=$!

export num_cores=`grep -c processor /proc/cpuinfo`

cd OpenBLAS

# make clean
make -j${num_cores} >> $BUILD_OUTPUT 2>&1
make install PREFIX=$PWD >> $BUILD_OUTPUT 2>&1

cd ../Ipopt

cd ThirdParty

chmod 777 -R ASL
chmod 777 -R Blas
chmod 777 -R Lapack
chmod 777 -R Mumps
chmod 777 -R Metis
chmod 777 -R HSL

cd ASL
# ./get.ASL

cd ../Blas
# ./get.Blas

cd ../Lapack
# ./get.Lapack

cd ../Mumps
# ./get.Mumps

cd ../Metis
# ./get.Metis

cd ../..

./configure --prefix=$PWD --with-blas="-L$PWD/../OpenBLAS/lib -lopenblas" --with-lapack="-L$PWD/../OpenBLAS/lib -lopenblas" --with-mumps=no --with-asl=no --with-pardiso=no ADD_CFLAGS=-fopenmp ADD_FFLAGS=-fopenmp ADD_CXXFLAGS=-fopenmp >> $BUILD_OUTPUT 2>&1
make clean >> $BUILD_OUTPUT 2>&1
make -j${num_cores} >> $BUILD_OUTPUT 2>&1
make install >> $BUILD_OUTPUT 2>&1

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../alglib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. .. >> $BUILD_OUTPUT 2>&1
make clean >> $BUILD_OUTPUT 2>&1
make -j${num_cores} >> $BUILD_OUTPUT 2>&1

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../../clp
./configure --prefix=$PWD >> $BUILD_OUTPUT 2>&1
make clean >> $BUILD_OUTPUT 2>&1
make -j${num_cores} >> $BUILD_OUTPUT 2>&1
make install >> $BUILD_OUTPUT 2>&1

if [ $? -ne 0 ] ; then
    exit 1
fi

cd ../portfolio-optimizer
mkdir build
cd build

if [ "$BUILD_TEST" = "ON" ] ; then
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. -DTEST=ON .. >> $BUILD_OUTPUT 2>&1
    make clean >> $BUILD_OUTPUT 2>&1
    make -j${num_cores} >> $BUILD_OUTPUT 2>&1

    cd ../bin
    ./test_suite
else
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/.. .. >> $BUILD_OUTPUT 2>&1
    make clean >> $BUILD_OUTPUT 2>&1
    make -j${num_cores} >> $BUILD_OUTPUT 2>&1

    if [ $? -ne 0 ] ; then
        exit 1
    fi
fi

cd ../..

# copy necessary lib

if [ ! -d lib ]; then
    mkdir lib
else
    rm -r lib/*
fi

cp alglib/lib/*.so lib/
cp clp/lib/*.so* lib/
cp OpenBLAS/lib/*.so* lib/
cp Ipopt/lib/*.so* lib/
cp portfolio-optimizer/lib/*.so* lib

# copy necessary header files
if [ ! -d include ]; then
    mkdir include
else
    rm -r include/*
fi

mkdir include/alglib
mkdir include/clp
mkdir include/ipopt
mkdir include/eigen
mkdir include/pfopt

cp alglib/src/*.h include/alglib
cp -r clp/include/* include/clp
cp -r Ipopt/include/* include/ipopt
cp -r eigen/Eigen include/eigen
cp portfolio-optimizer/pfopt/*.hpp include/pfopt

# build java interface for ipopt, please see README.md for detail

cd Ipopt/

cd Ipopt/contrib/JavaInterface

export CLASSPATH=$PWD:$CLASSPATH
export CPLUS_INCLUDE_PATH=$JAVA_HOME/include/linux

make >> $BUILD_OUTPUT 2>&1

if [ $? -ne 0 ] ; then
    exit 1
fi

echo "Portfolio - Optimizer building completed!"

# The build finished without returning an error so dump a tail of the output
dump_output

# nicely terminate the ping output loop
kill $PING_LOOP_PID
