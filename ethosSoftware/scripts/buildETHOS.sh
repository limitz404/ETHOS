#! /bin/bash
g++ -c -I ../src/common/ ../src/algorithm/edgeDetection.cpp -std=c++0x
g++ -c -I ../src/common/ ../src/algorithm/attitudeDetermination.cpp -std=c++0x
g++ -c -I ../src/common/ ../src/common/common.cpp
g++ -c -I ../src/bitBangTesting/ -I ../src/common/ -I ../src/algorithm/ ../src/common/main.cpp -std=c++0x

gcc -I ../src/common/ -I /usr/include/ -L /usr/lib/ -c ../src/bitBangTesting/bitbang.c -lpthread -lprussdrv

g++ -I /usr/include/ -L /usr/lib/ common.o edgeDetection.o attitudeDetermination.o main.o bitbang.o -lpthread -lprussdrv

pasm -V3 -b ../src/bitBangTesting/bitBang.p

/bin/mv ./bitBang.bin ../bin/
/bin/mv ./*.o ../obj/
/bin/mv ./a.out ../bin/ethos