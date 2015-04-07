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
#include "attitudeDetermination.h"
#include "dataStructures.h"
#include "common.h"


/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

extern "C" {
    #include "bitBangController.h"
}

#define NUMROWS 128
#define NUMCOLS 162


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*
struct canFlags{
    int sendHealth;
    int sendDisp;
    int sendLog;
};
*/

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

/*
struct canFlags canDump();


void canHandler( struct attitude  displacement; float health ){


    int check = 1;

    while( check ){
        flag = canDump();
        switch( flag 
    }

}
*/

int main( int argc, char *argv[] )
{

    FILE * filePtr;

    int single;

    /* chech for single frame mode */
    if( argc > 1 ){
        single = 1;
        filePtr = fopen( "/root/ethosSoftware/output/image.txt", "w" );
        if( filePtr == NULL ){
            printf("Unable to create 'image.txt'");
            return(-1);
        }
    } else {
        single = 0;
    }

    /* declare image */
    unsigned int image[NUMROWS][NUMCOLS];

    /* create empty log file file */
    FILE * logPtr;
    logPtr = fopen( "/root/ethosSoftware/output/logFile.bin", "w" );
    if( logPtr == NULL ){
        printf("Unable to create 'logFile.bin'");
        return(-1);
    }


    float floatBuffer[2];

    /* main while loop */
    int loopVar = 0;
    while( loopVar < 100 ){

        //
        /* get current image */
        doBitBang(image);

        //
        /* calculate displacement */
        attitude finalAtt = determineAttitude(image);

        /* flag bad data (outside limits or changed too much) */
        /* check CAN for requests */
        /* send displacement if requested */

        //
        /* get current health telemetry */
        /* send health if requested */

        //
        /* write data to file */
        floatBuffer[0] = finalAtt.roll;
        floatBuffer[1] = finalAtt.pitch;
        fwrite( floatBuffer, sizeof(char), sizeof(floatBuffer), logPtr );

        printf("%u\t%f\t%f\n",loopVar++,finalAtt.roll, finalAtt.pitch);

        if( single == 1 ){
            int col;
            int line;
            for (line = 0; line < NUMROWS; line++){
                for (col = 0; col < NUMCOLS; col++){
                    fprintf(filePtr, "%i\t", (int) image[line][col] & 0xff);
                }
                fprintf(filePtr,"\n");
            }
            fclose(filePtr);
            break;
        }

        loopVar++;

    }

    fclose(logPtr);
    return 0;

}
