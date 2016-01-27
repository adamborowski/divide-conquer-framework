#!/bin/bash
echo $1
source /opt/intel/composer_xe_2013_sp1.3.174/bin/compilervars.sh intel64




ssh mic0 '
    mkdir aborowski;
    cd aborowski;
    rm divide_conquer_framework;
    '

scp /opt/intel/composer_xe_2013_sp1.3.174/compiler/lib/mic/libiomp5.so mic0:~/aborowski/
scp divide_conquer_framework mic0:~/aborowski/
