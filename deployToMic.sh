#!/bin/bash
echo $1
source /opt/intel/composer_xe_2013_sp1.3.174/bin/compilervars.sh intel64
rm -f program.out

icc -openmp -O3 -mmic -std=c++11 $1 -lrt -o program.out

ssh mic0 '
    mkdir mszerszynski;
    cd mszerszynski;
    rm program.out;
    '

scp /opt/intel/composer_xe_2013_sp1.3.174/compiler/lib/mic/libiomp5.so mic0:~/mszerszynski
scp program.out mic0:~/mszerszynski

if [ ! -f program.out ]; then
    echo "Executable not created, aborting!"
else
    ssh mic0 '
    cd mszerszynski;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.;
    time ./program.out 0.1 10
    time ./program.out 0.1 50
    time ./program.out 0.1 100
    time ./program.out 0.1 500
    time ./program.out 0.1 1000

    time ./program.out 0.05 10
    time ./program.out 0.05 50
    time ./program.out 0.05 100
    time ./program.out 0.05 500
    time ./program.out 0.05 1000

    time ./program.out 0.01 10
    time ./program.out 0.01 50
    time ./program.out 0.01 100
    time ./program.out 0.01 500
    '

fi