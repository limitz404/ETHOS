##########################################################################################
# AUTHORS:      PATRICK KLEIN, TAYLOR DEAN                                               #
# FILE:         buildEthos.sh                                                            #
# CREATED:      03-29-2015                                                               #
# ORGANIZATION: CU-BOULDER, AES SENIOR DESIGN TEAM ETHOS                                 #
# PURPOSE:      COMPILES, LINKS, AND ASSEMBLES SOURCE CODE                               #
# options:      clean                                                                    #
#               test                                                                     #
#               -Wall, warnings                                                          #
#               -O1 [all]                                                                #
#               -O2 [all]                                                                #
#               -O3 [all]                                                                #
#               debug, -g                                                                #
#               profile, -pg                                                             #
# NOTES:        INDEPENDENT OF CURRENT DIRECTORY                                         #
#               OPTIONS MUST BE LOWER-CASE                                               #
##########################################################################################

#!/bin/bash

INC=/usr/include/
LIB=/usr/lib/

SRC=/root/ethosSoftware/src
OBJ=/root/ethosSoftware/obj
OUT=/root/ethosSoftware/output
BIN=/root/ethosSoftware/bin

OPTIM_OPTIONS=""

COMPILE_OPTIONS=""
LINK_OPTIONS=""

TRUE=0

# evaluate input arguments
for arg in $@
do

    case "$arg" in

    "clean" )    rm $BIN/* > /dev/null 2>&1
                 rm $OUT/*.* > /dev/null 2>&1
                 rm $OBJ/* > /dev/null 2>&1
                 echo "clean"
                 ;;

    "test" )     TEST=1
                 echo "test"
                 ;;

    "-O1" )       OPTIM_OPTIONS="$OPTIM_OPTIONS -O1"
                  echo "-O1"
                  ;;

    "-O2" )       OPTIM_OPTIONS="$OPTIM_OPTIONS -O2"
                  echo "-O2"
                  ;;

    "-O3" )       OPTIM_OPTIONS="$OPTIM_OPTIONS -O3"
                  echo "-O3"
                  ;;

    "all" )       COMPILE_OPTIONS="$COMPILE_OPTIONS $OPTIM_OPTIONS"
                  OPTIM_OPTIONS=""
                  echo "all"
                  ;;

    "debug" )     COMPILE_OPTIONS="$COMPILE_OPTIONS -g"
                  LINK_OPTIONS="$LINK_OPTIONS -g"
                  echo "debug"
                  ;;

    "-g" )        COMPILE_OPTIONS="$COMPILE_OPTIONS -g"
                  LINK_OPTIONS="$LINK_OPTIONS -g"
                  echo "-g"
                  ;;

    "profile" )    COMPILE_OPTIONS="$COMPILE_OPTIONS -pg"
                   LINK_OPTIONS="$LINK_OPTIONS -pg"
                   echo "profile"
                   ;;

    "-pg" )        COMPILE_OPTIONS="$COMPILE_OPTIONS -pg"
                   LINK_OPTIONS="$LINK_OPTIONS -pg"
                   echo "-pg"
                   ;;

    "warnings" )   COMPILE_OPTIONS="$COMPILE_OPTIONS -Wall"
                   LINK_OPTIONS="$LINK_OPTIONS -Wall"
                   echo "warnings"
                   ;;

    "-Wall" )      COMPILE_OPTIONS="$COMPILE_OPTIONS -Wall"
                   LINK_OPTIONS="$LINK_OPTIONS -Wall"
                   echo "-Wall"
                   ;;

    * )           echo "Unrecognized command: $arg"
                  echo "Note: commands must be lower case"
                  ;;

    esac

done

# script is broken?  override test for now
TEST=0

# compile C++ source files and place into OBJ
g++ -c -I $SRC/common/ -o $OBJ/edgeDetection.o $SRC/algorithm/edgeDetection.cpp -std=c++0x $COMPILE_OPTIONS
g++ -c -I $SRC/common/ -o $OBJ/attitudeDetermination.o $SRC/algorithm/attitudeDetermination.cpp -std=c++0x $COMPILE_OPTIONS
g++ -c -I $SRC/common/ -o $OBJ/common.o $SRC/common/common.cpp $COMPILE_OPTIONS
g++ -c -I $SRC/bitBang/ -I $SRC/common/ -I $SRC/algorithm/ -o $OBJ/main.o $SRC/common/main.cpp -std=c++0x $COMPILE_OPTIONS

# compile C source file and place into OBJ
gcc -c -I $SRC/common/ -I $INC -L $LIB -o $OBJ/bitBangController.o $SRC/bitBang/bitBangController.c -lpthread -lprussdrv $COMPILE_OPTIONS

# assemble PRU code and place into OBJ
if [ $TEST -eq 1 ]
then
    pasm -V3 -b $SRC/bitBang/bitBangTesting.p $BIN/bitBang > $OUT/pasmLog.txt
else
    pasm -V3 -b $SRC/bitBang/bitBang.p $BIN/bitBang > $OUT/pasmLog.txt
    echo "for real"
fi

# link object files and place into BIN
g++ -I $INC -L $LIB $OBJ/common.o $OBJ/edgeDetection.o $OBJ/attitudeDetermination.o $OBJ/main.o $OBJ/bitBangController.o -o $BIN/ethos -lpthread -lprussdrv $LINK_OPTIONS
