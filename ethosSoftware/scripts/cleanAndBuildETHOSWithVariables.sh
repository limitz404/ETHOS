##########################################################################################
# AUTHORS:      TAYLOR DEAN, PATRICK KLEIN                                               #
# FILE:         buildEthos.sh                                                            #
# CREATED:      03-29-2015                                                               #
# ORGANIZATION: CU-BOULDER, AES SENIOR DESIGN TEAM ETHOS                                 #
# PURPOSE:      COMPILES, LINKS, AND ASSEMBLES SOURCE CODE                               #
# NOTES:        CURRENTLY COMPILES TEST BITBANG CODE (NOT RELIANT ON IR CAMERA),         #
#               INDEPENDENT OF CURRENT DIRECTORY                                         #
##########################################################################################


#! /bin/bash

# clean
rm /root/ethosSoftware/bin/*
rm /root/ethosSoftware/obj/*

INC = /usr/include/
LIB = /usr/lib/

SRC = /root/ethosSoftware/src
OBJ = /root/ethosSoftware/obj
OUT = /root/ethosSoftware/output
BIN = /root/ethosSoftware/bin

# compile C++ source files and place into OBJ
g++ -c -I $SRC/common/ -o $OBJ/edgeDetection.o $SRC/algorithm/edgeDetection.cpp -std=c++0x -g
g++ -c -I $SRC/common/ -o $OBJ/attitudeDetermination.o $SRC/algorithm/attitudeDetermination.cpp -std=c++0x -g
g++ -c -I $SRC/common/ -o $OBJ/common.o SRC/common/common.cpp -g
g++ -c -I $SRC/bitBangTesting/ -I $SRC/common/ -I $SRC/algorithm/ -o $OBJ/main.o $SRC/common/main.cpp -std=c++0x -g

# compile C source file and place into OBJ
gcc -I $SRC/common/ -I $INC -L $LIB -c -o $OBJ/bitBangController.o $SRC/bitBangTesting/bitBangController.c -lpthread -lprussdrv -g

# assemble PRU code and place into OBJ
pasm -V3 -b SRC/bitBangTesting/bitBang.p OBJ/bitBang > $OUT/pasmLog.txt

# link object files and place into BIN
g++ -I $INC -L $LIB $OBJ/common.o $OBJ/edgeDetection.o $OBJ/attitudeDetermination.o $OBJ/main.o $OBJ/bitBangController.o -o $BIN/ethos -lpthread -lprussdrv -g
