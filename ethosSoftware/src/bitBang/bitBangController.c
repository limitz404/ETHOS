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

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "bitBangController.h"

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define NUMCOLS	    162
#define NUMROWS     128

#define PRU_NUM     1
#define AM33XX

/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

int numCols = 162;
int numRows = 128;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/



int initializePRU( void )
{

    /* Initialize the PRU */
    prussdrv_init ();

    /* Open PRU Interrupt */             //DO YOU REALLY NEED THIS?
    int ret = prussdrv_open(PRU_EVTOUT_1);
    if( ret ){
        printf("prussdrv_open open failed\n");
        return (ret);
    }

    /* Execute PRU code */
    prussdrv_exec_program ( PRU_NUM, "/root/ethosSoftware/bin/bitBang.bin" );

    return(0);
}




int getImage( int frameData[][NUMCOLS] )
{

    static void *pruDataMem;
    static volatile unsigned int *pruDataMem_int;

    /* Initialize the data memory */
    prussdrv_map_prumem( PRUSS0_PRU1_DATARAM , &pruDataMem );
    pruDataMem_int = (unsigned int*) pruDataMem;

    // Flush the values in the PRU data memory locations
    int i;
    for(i = 0; i< 12*162; i++){
      pruDataMem_int[i] = 0x00000000;
    }

    unsigned int ack, line=0;
    int regBlock, regMax, reg;

    /* write to ack */
    pruDataMem_int[0] = 0x80000000;

    /* bit-bang frame */
    while( line < numRows ) {
        if ( (pruDataMem_int[0] && 0xff) != 0 ){    // MIGHT NOT BE ENOUGH TIME FOR PRU TO ALWAYS CLEAR ACK
            ack = pruDataMem_int[0];
            pruDataMem_int[0] = 0;
            line = ack & 0xFF;
            regBlock = ack>>8;
            regMax = (regBlock+1)*numCols+1;
            i = 0;
            for (reg = regBlock*numCols+1; reg < regMax; reg++){
                frameData[line-1][i++] = pruDataMem_int[reg] & 0xff;
            }
        }
    }

    return(0);
}


void closePRU( void )
{
    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit();
    return;
}
