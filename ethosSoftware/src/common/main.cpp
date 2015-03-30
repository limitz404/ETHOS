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


/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/


int main()
{


    int image[NUMROWS][NUMCOLS];

    /* create empty file */
    FILE * filePtr;
    filePtr = fopen( "/root/ethosSoftware/output/logFile.txt", "w" );
    if( filePtr == NULL ){
        printf("Unable to create 'logFile.txt'");
        return(-1);
    }

    float floatBuffer[2];

    int loopVar = 0;
    while( loopVar < 100 ){

        //

        /* get current image */
        doBitBang(image);


        //

        /* calculate displacement */
        attitude finalAtt = determineAttitude(image);
        printf("%i\t%f\t%f\n",loopVar,finalAtt.roll, finalAtt.pitch);

        /* flag bad data (outside limits or changed too much) */

        /* check CAN for requests */

        /* send displacement if requested */


        //

        /* get current health telemetry */

        /* send health if requested */


        //

        /* write data to file
        floatBuffer[0] = finalAtt.roll;
        floatBuffer[1] = finalAtt.pitch;
        fwrite( floatBuffer, sizeof(char), sizeof(floatBuffer), filePtr );
        */
        //fprintf(filePtr, "%u\t%f\t%f\n",loop++,finalAtt.roll, finalAtt.pitch);
        loopVar++;
        printf("debug\n");

    }

    printf("debug\n");
    fclose(filePtr);
    return 0;

}
