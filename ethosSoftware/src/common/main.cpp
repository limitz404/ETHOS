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

#define LOG_TIME 60*1
#define DATA_RATE 1


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

/* parse log to return latest values */
int parseLog(struct attitude *logAtt)
{

    FILE * oldLogPtr;
    oldLogPtr = fopen( "/root/ethosSoftware/output/logFile.txt" , "r" );
    if( oldLogPtr == NULL ){
        return(-1);
    }

    printf("Starting to parse 'logFile.txt'...\n");

    int valid = 0x01;
    unsigned int num, prevNum = 0;
    float roll, pitch;

    do {
        valid &= fscanf( oldLogPtr , "%u" , &num );
        valid &= fscanf( oldLogPtr , "%f" , &roll );
        valid &= fscanf( oldLogPtr , "%f" , &pitch );
        if( num > prevNum ){
            prevNum = num;
            (*logAtt).roll = roll;
            (*logAtt).pitch = pitch;
        }
    } while ( valid );


    return(0);
}



int main( int argc, char *argv[] )
{

    int single = 0, save = 0;

    /* parse old log file
    attitude oldVals;
    int ret = parseLog( &oldVals );
    if( ret ){
        printf("Unable to read archived 'logFile.txt'\n");
    } else {
        printf("Most recent values:\t%f\t%f\n", oldVals.roll, oldVals.pitch);
    }
    */

    /* chech for single frame mode */
    if( argc > 2){
        save = 1;
        single = 1;
    }else if( argc > 1 ){
        save = 1;
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


    //float floatBuffer[2];

    /* declare variables outside of loop */
    FILE * filePtr;
    attitude finalAtt;
    unsigned int loopVar = 0;
    unsigned int lineCounter = 0;
    clock_t refTime = clock();


    /* main do-while loop */
    do {

        //
        /* get current image */
        doBitBang(image);

        //
        /* calculate displacement */
        //finalAtt = determineAttitude(image);

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

        /* cycle back to beginning of file every LOG_TIME seconds */
        if( !single && (lineCounter/DATA_RATE > LOG_TIME) ){
            lineCounter = 0;
            rewind(logPtr);
            clock_t endTime = clock();
            printf("Looping back to beginning of log file (%f minutes)\n", loopVar, (float) (endTime-refTime)/CLOCKS_PER_SEC/60);
        }

        /* write data to file */
        fprintf(logPtr, "%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);

        if( save == 1 ){
            char stringBuffer[50];
            sprintf(stringBuffer, "/root/ethosSoftware/output/image%u.txt", loopVar);
            filePtr = fopen( stringBuffer, "w" );
            if( filePtr == NULL ){
                printf("Unable to create 'image%u.txt'", loopVar);
                return(-1);
            }
            int col;
            int line;
            for (line = 0; line < NUMROWS; line++){
                for (col = 0; col < NUMCOLS; col++){
                    fprintf(filePtr, "%i\t", (int) image[line][col]);
                }
                fprintf(filePtr,"\n");
            }
            fclose(filePtr);
        }

        loopVar++;
        lineCounter++;

        /* break if single mode */
        //if( single == 1 ){
        //    break;
        //}

    } while ( !single && (loopVar < 3*LOG_TIME) );

    /* close logFile.txt */
    fclose(logPtr);

    /* final iteration/calculation */
    printf("%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);

    /* show total calculation rate (for DATA_RATE calculations) */
    clock_t endTime = clock();
    printf("%u frames processed in %f seconds\n", loopVar, (float) (endTime-refTime)/CLOCKS_PER_SEC);

    return 0;

}
