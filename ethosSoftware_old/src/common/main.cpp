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
#include <time.h>
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

#define LOG_TIME 60*200
#define DATA_RATE 7


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

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

    int image[NUMROWS][NUMCOLS];

    /* create empty log file file */
    FILE * logPtr;
    logPtr = fopen( "/root/ethosSoftware/output/logFile.txt", "w+" );
    if( logPtr == NULL ){
        printf("Unable to create 'logFile.txt'");
        return(-1);
    }


    float floatBuffer[2];

    //clock_t refTime = clock();

    /* main while loop */
    unsigned int loopVar = 0;
    unsigned int lineCounter = 0;
    while( 1 ){

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
        //floatBuffer[0] = finalAtt.roll;
        //floatBuffer[1] = finalAtt.pitch;
        //fwrite( floatBuffer, sizeof(char), sizeof(floatBuffer), logPtr );

        printf("%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);

        /* cycle back to beginning of file every 200 minutes */
        /** option to use absolute time or based on average data rate **/
        // if( ( clock()-refTime )/CLOCKS_PER_SEC > LOG_TIME ){
        if( ~single && ( lineCounter/DATA_RATE > LOG_TIME) ){
            //refTime = clock();
            lineCounter = 0;
            rewind(logPtr);
            return(0); // let's break for now
        }

        /* write data to file */
        fprintf(logPtr, "%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);

        /* save image and break if single mode */
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
        lineCounter++;

    }

    fclose(logPtr);
    return 0;

}
