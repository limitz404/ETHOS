#! /bin/bash
g++ -I /usr/local/include/ -L /usr/local/lib/  main.cpp -o EdgeDetection -lopencv_core -lopencv_highgui -std=c++0x