##########################################################################################
# AUTHORS:      PATRICK KLEIN, TAYLOR DEAN                                               #
# FILE:         buildEthos.sh                                                            #
# CREATED:      03-29-2015                                                               #
# ORGANIZATION: CU-BOULDER, AES SENIOR DESIGN TEAM ETHOS                                 #
# PURPOSE:      COMPILES, LINKS, AND ASSEMBLES SOURCE CODE                               #
# NOTES:        CURRENTLY COMPILES TEST BITBANG CODE (NOT RELIANT ON IR CAMERA),         #
#               INDEPENDENT OF CURRENT DIRECTORY                                         #
##########################################################################################

# options:    clean
#             optimize
#             debugging
#             profiling

#!/bin/bash

INC=/usr/include/
LIB=/usr/lib/

SRC=/root/ethosSoftware/src
OBJ=/root/ethosSoftware/obj
OUT=/root/ethosSoftware/output
BIN=/root/ethosSoftware/bin

COMPILE_OPTIONS=""
LINK_OPTIONS=""

# evaluate input arguments
for arg in $@
do

    case "$arg" in

    "clean" )    rm $BIN/*
                 rm $OUT/*.*
                 rm $OBJ/*
                 ;;

    "optimize1" )   COMPILE_OPTIONS="$COMPILE_OPTIONS -O1"
                    LINK_OPTIONS="$LINK_OPTIONS -O1"
                    ;;

    "optimize2" )   COMPILE_OPTIONS="$COMPILE_OPTIONS -O2"
                    LINK_OPTIONS="$LINK_OPTIONS -O2"
                    ;;

    "optimize3" )   COMPILE_OPTIONS="$COMPILE_OPTIONS -O3"
                    LINK_OPTIONS="$LINK_OPTIONS -O3"
                    ;;

    "debug" )    COMPILE_OPTIONS="$COMPILE_OPTIONS -g"
                 LINK_OPTIONS="$LINK_OPTIONS -g"
                 ;;

    "profile" )    COMPILE_OPTIONS="$COMPILE_OPTIONS -pg"
                   LINK_OPTIONS="$LINK_OPTIONS -pg"
                   ;;

    * )    echo "Unrecognized command: $arg"
                   ;;

    esac

done

# compile C++ source files and place into OBJ
g++ -c -I $SRC/common/ -o $OBJ/edgeDetection.o $SRC/algorithm/edgeDetection.cpp -std=c++0x $COMPILE_OPTIONS
g++ -c -I $SRC/common/ -o $OBJ/attitudeDetermination.o $SRC/algorithm/attitudeDetermination.cpp -std=c++0x $COMPILE_OPTIONS
g++ -c -I $SRC/common/ -o $OBJ/common.o $SRC/common/common.cpp $COMPILE_OPTIONS
g++ -c -I $SRC/bitBangTesting/ -I $SRC/common/ -I $SRC/algorithm/ -o $OBJ/main.o $SRC/common/main.cpp -std=c++0x $COMPILE_OPTIONS

# compile C source file and place into OBJ
gcc -I $SRC/common/ -I $INC -L $LIB -c -o $OBJ/bitBangController.o $SRC/bitBangTesting/bitBangController.c -lpthread -lprussdrv $COMPILE_OPTIONS

# assemble PRU code and place into OBJ
pasm -V3 -b SRC/bitBangTesting/bitBang.p OBJ/bitBang > $OUT/pasmLog.txt

# link object files and place into BIN
g++ -I $INC -L $LIB $OBJ/common.o $OBJ/edgeDetection.o $OBJ/attitudeDetermination.o $OBJ/main.o $OBJ/bitBangController.o -o $BIN/ethos -lpthread -lprussdrv $LINK_OPTIONS



