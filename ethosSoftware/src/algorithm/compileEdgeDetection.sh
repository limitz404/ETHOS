#! /bin/bash
g++ -I /usr/local/include/ -L /usr/local/lib/ edgeDetection.cpp attitudeDetermination.cpp main.cpp -lopencv_core -lopencv_highgui -std=c++0x
/bin/mv a.out ../../bin/ethos