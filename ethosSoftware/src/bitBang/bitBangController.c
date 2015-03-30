/*****************************************************************************
* AUTHORS: PATRICK KLEIN, TAYLOR DEAN                                        *
* TEAM: ETHOS                                                                *
* FILE: bitBangController.c                                                  *
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

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define PRU_NUM     1
#define NUM_ROWS    128
#define NUM_COLS    162
#define MAX_REG     2047
#define AM33XX

/*****************************************************************************
* Local Function Declarations                                                *
*****************************************************************************/

int dramInitialization();

/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

int numCols = NUM_COLS;
int numRows = NUM_ROWS;

unsigned int frameData[NUM_ROWS][NUM_COLS];
static void *pruDataMem;
static unsigned int *pruDataMem_int;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

int main (void)
{
    printf("\nLet's Bitbang! Frame %u\n",frame);

    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    /* Initialize the PRU */
    prussdrv_init ();

    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_1);

    if (ret){
        printf("prussdrv_open open failed\n");
        return (ret);
    }

    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);

    /* Initialize the data memory */
    dramInitialization();

    /* Execute PRU code */
    prussdrv_exec_program(PRU_NUM, "/root/ethosSoftware/bin/bitBang.bin");

    //printf("Beginning data acquisition...\n");
    unsigned int ack;
    int line = 0;
    int regblock;
    int regmax;
    int i;
    int reg;
    while ( line < numRows ){
        if ( pruDataMem_int[0] != 0 ){
            ack = pruDataMem_int[0];
            pruDataMem_int[0] = 0;
            line = ack & 0xFF;
            regblock = ack>>8;
            regmax = (regblock+1)*numCols+1;
            i = 0;
            for (reg = regblock*numCols+1; reg < regmax; reg++){
                frameData[line][i++] = pruDataMem_int[reg];
            }
        }
    }

    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit();

    return(0);
}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/

// Initialize the PRU data ram by writing 0s to the registers
int dramInitialization()
{
    int i;
    prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, &pruDataMem);
    pruDataMem_int = (unsigned int*) pruDataMem;

    // Flush the values in the PRU data memory locations
    for(i = 0; i<2048; i++){
      pruDataMem_int[i] = 0x00000000;
    }

    return(0);
}
