/*****************************************************************************
* AUTHORS: PATRICK KLEIN, TAYLOR DEAN                                        *
* TEAM: ETHOS                                                                *
* FILE: bitbang.c                                                  *
* CREATED: 3 MARCH 2015                                                      *
* PURPOSE: TO LOAD BITBANG ASSEMBLY CODE ONTO PRU 1, START THE CODE, THEN    *
*          GRAB IMAGE DATA FROM THE PRU DATA RAM.                            *
*****************************************************************************/

/*****************************************************************************
* Include Files                                                              *
*****************************************************************************/

#include <iostream>
#include <stdio.h>

#include "readImage.h"

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define NUM_ROWS 128
#define NUM_COLS 162


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/



int getImage( int image[][NUM_COLS] )
{

    FILE * filePtr;
    filePtr = fopen( "/root/ethosSoftware/requiredFiles/0r0p.txt" , "rb" );
    if( filePtr == NULL ){
        return(-1);
    }
    int col;
    int line;
    for( line = 0; line < NUM_ROWS; line++ ){
        for( col = 0; col < NUM_COLS; col++ ){
            fscanf(filePtr, "%i", (int) image[line][col]);
        }
    }
    fclose(filePtr);

    return(0);

}
