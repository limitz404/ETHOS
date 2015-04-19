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
#include "readImage.h"
#include "uart.h"


/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/


#define NUM_ROWS 128
#define NUM_COLS 162

#define LOG_TIME 60*200    // FINAL PRODUCT SHOULD BE SET TO 60*200
#define DATA_RATE 7.5      // CURRENTLY ABOUT 7.5 HZ


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/


int main( int argc, char *argv[] )
{

    /* parse old log file
    attitude oldVals;
    int ret = parseLog( &oldVals );
    if( ret ){
        printf("Unable to read archived 'logFile.txt'\n");
    } else {
        printf("Most recent values:\t%f\t%f\n", oldVals.roll, oldVals.pitch);
    }
    */

    /* chech for save & single-frame mode */
    int single=0 , save=0;
    if( argc > 2){
        save = 1;
        single = 1;
    }else if( argc > 1 ){
        save = 1;
    }

    /* declare image */
    int image[NUM_ROWS][NUM_COLS];

    /* create empty log file file */
    FILE * logPtr;
    logPtr = fopen( "/root/ethosSoftware/output/logFile.txt", "w+" );
    if( logPtr == NULL ){
        printf("Unable to create 'logFile.txt'");
        return(-1);
    }

    /* initialize UART
    printf("intializing UART\n");
    initUart ();
    */

    /* declare variables outside of loop */
    FILE * filePtr;
    FILE * tempPtr;
    attitude finalAtt;
    float health[2];
    float temp;
    unsigned int loopVar=0, lineCounter=0;
    clock_t refTime = clock();


    printf("running mainloop\n");
    /* main do-while loop */
    do {

        /* get current image */
        getImage(image);


        /* calculate displacement */
        finalAtt = determineAttitude(image);

        /* flag bad data (outside limits or changed too much) */


        /* get current health telemetry */
        /* health = getHealth(); */
        health[0] = 0xff00ff00;
        tempPtr = fopen( "/sys/class/hwmon/hwmon0/device/temp1_input" , "r");
        fscanf( tempPtr , "%f" , &temp );
        fclose( tempPtr );
        health[1] = temp*0.001;
        //printf("%f\n" , temp);


        /* check and respond to UART
        printf("checking UART\n");
        uartHandler( &finalAtt ,  &health );
        */


        /* display current line for testing purposes */
        //printf("%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);


        /* cycle back to beginning of file every LOG_TIME seconds */
        if( !single && (lineCounter/DATA_RATE > LOG_TIME) ){
            lineCounter = 0;
            rewind(logPtr);
            clock_t endTime = clock();
            printf("Looping back to beginning of log file (%f minutes)\n", loopVar, (float) (endTime-refTime)/CLOCKS_PER_SEC/60);    // REMOVE WHEN DONE TESTING
        }

        /* write data to file */
        fprintf(logPtr, "%u\t%f\t%f\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch, health[0], health[1]);

        /* save image to text file if requested */
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
            for( line = 0; line < NUM_ROWS; line++ ){
                for (col = 0; col < NUM_COLS; col++ ){
                    fprintf(filePtr, "%i\t", (int) image[line][col]);
                }
                fprintf(filePtr,"\n");
            }
            fclose(filePtr);
        }

        /* increment loop variables */
        loopVar++;
        lineCounter++;

    } while ( !single && (loopVar < 1.5*LOG_TIME*DATA_RATE) );    // FINAL PRODUCT WILL HAVE INFINITE WHILE LOOP
    //} while ( !single );

    /* close logFile.txt */
    fclose(logPtr);

    /* final iteration/calculation */
    printf("%u\t%f\t%f\n", loopVar, finalAtt.roll, finalAtt.pitch);

    /* show total calculation rate (for DATA_RATE calculations) */
    clock_t endTime = clock();
    printf("%u frames processed in %f seconds\n", loopVar, (float) (endTime-refTime)/CLOCKS_PER_SEC);

    return(0);

}






/* parse log to return latest values */
int parseLog(struct attitude *logAtt)
{

    /* re-open log file if it exists */
    FILE * oldLogPtr;
    oldLogPtr = fopen( "/root/ethosSoftware/output/logFile.txt" , "r" );
    if( oldLogPtr == NULL ){
        return(-1);
    }

    /* verify parsing is beginning */
    printf("Starting to parse 'logFile.txt'...\n");

    /* declare variables outside loop */
    int valid = 0x01;
    int num , prevNum=-1;
    float roll, pitch;

    /* loop through entire file */
    do {
        valid &= fscanf( oldLogPtr , "%u" , &num );
        valid &= fscanf( oldLogPtr , "%f" , &roll );
        valid &= fscanf( oldLogPtr , "%f" , &pitch );
        if( num > prevNum ){
            prevNum = num;
            (*logAtt).roll = roll;
            (*logAtt).pitch = pitch;
            printf( "Lines read...\t%i" , num );
        }
    } while ( valid );


    return(0);
}
