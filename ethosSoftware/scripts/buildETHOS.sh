#! /bin/bash
g++ -I /usr/local/include/ -L /usr/local/lib/ ../src/algorithm/edgeDetection.cpp ../src/algorithm/attitudeDetermination.cpp ../src/algorithm/main.cpp -lopencv_core -lopencv_highgui -std=c++0x
/bin/mv ./a.out ../bin/ethos