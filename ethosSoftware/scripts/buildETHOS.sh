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

# compile C++ source files and place into ~/ethosSoftware/obj/
g++ -c -I /root/ethosSoftware/src/common/ -o /root/ethosSoftware/obj/edgeDetection.o /root/ethosSoftware/src/algorithm/edgeDetection.cpp -std=c++0x -g
g++ -c -I /root/ethosSoftware/src/common/ -o /root/ethosSoftware/obj/attitudeDetermination.o /root/ethosSoftware/src/algorithm/attitudeDetermination.cpp -std=c++0x -g
g++ -c -I /root/ethosSoftware/src/common/ -o /root/ethosSoftware/obj/common.o /root/ethosSoftware/src/common/common.cpp -g
g++ -c -I /root/ethosSoftware/src/bitBangTesting/ -I /root/ethosSoftware/src/common/ -I /root/ethosSoftware/src/algorithm/ -o /root/ethosSoftware/obj/main.o /root/ethosSoftware/src/common/main.cpp -std=c++0x -g

# compile C source file and place into ~/ethosSoftware/obj/
gcc -I ../src/common/ -I /usr/include/ -L /usr/lib/ -c -o /root/ethosSoftware/obj/bitBangController.o /root/ethosSoftware/src/bitBangTesting/bitBangController.c -lpthread -lprussdrv -g

# assemble PRU code and place into ~ethosSoftware/obj/
pasm -V3 -b /root/ethosSoftware/src/bitBangTesting/bitBang.p /root/ethosSoftware/obj/bitBang > /root/ethosSoftware/output/pasmLog.txt

# link object files and place into ~ethosSoftware/bin/
g++ -I /usr/include/ -L /usr/lib/ /root/ethosSoftware/obj/common.o /root/ethosSoftware/obj/edgeDetection.o /root/ethosSoftware/obj/attitudeDetermination.o /root/ethosSoftware/obj/main.o /root/ethosSoftware/obj/bitBangController.o -o /root/ethosSoftware/bin/ethos -lpthread -lprussdrv -g
