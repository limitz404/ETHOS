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
* Explicit External Declarations                                             *
*****************************************************************************/

/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

#define PRU_NUM     1
#define NUM_ROWS    162
#define NUM_COLS    128
#define MAX_REG     2048
#define MAX_IND     MAX_REG - NUM_COLS
#define AM33XX

/*****************************************************************************
* Local Typedef Declarations                                                 *
*****************************************************************************/


/*****************************************************************************
* Local Function Declarations                                                *
*****************************************************************************/

int dramInitialization();
int getData(int, int);

/*****************************************************************************
* Local Variable Definitions                                                 *
*****************************************************************************/


/*****************************************************************************
* Intertupt Service Routines                                                 *
*****************************************************************************/


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

unsigned int frameData[NUM_ROWS][NUM_COLS];
static void *pruDataMem;
static unsigned int *pruDataMem_int;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/

int main (void)
{
    printf("\nLet's Bitbang!\n");

    unsigned int ret;
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    /* Initialize the PRU */
    printf("Initializing the PRU...\n");
    prussdrv_init ();

    /* Open PRU Interrupt */
    printf("Opening PRU Interrupt...\n");
    ret = prussdrv_open(PRU_EVTOUT_1);
    
    if (ret){
        printf("prussdrv_open open failed\n");
        return (ret);
    }

    /* Get the interrupt initialized */
    printf("Get the interrupt initialized...\n");
    prussdrv_pruintc_init(&pruss_intc_initdata);

    /* Initialize the data memory */
    printf("Initializing Data RAM...\n");
    dramInitialization();

    /* Get first line */
    printf("Running PRU code for the first line...\n");
    prussdrv_exec_program ( PRU_NUM, "./bitBang.bin");
    printf("Waiting for halt...\n");
    

    /* Get subsequent lines */
    int line;
    int startInd = 0;
    for (line = 0; line < NUM_ROWS; line++){
        printf("Running PRU code for line %d...\n",line);
        
	//printf("Waiting for halt...\n");
        prussdrv_pru_wait_event(PRU_EVTOUT_1);
        prussdrv_pru_clear_event(PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
	//printf("Getting data.../n");
        getData(line, startInd);
        startInd = startInd + NUM_COLS;
        
        if (startInd >= MAX_IND){
            startInd = 0;
        }
    }

    /* Disable PRU and close memory mapping*/
    printf("Disabling PRU and closing memory mapping...\n");
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit();

    /* Displaying data... */
    FILE *fid = fopen("file.txt","w");

    int col;
    for (line = 0; line < NUM_ROWS; line++){
        for (col = 0; col < NUM_COLS; col++){
            if ( ( frameData[line][col] & 0xff ) != 0xff || 1){
	        fprintf(fid, "%u\t", frameData[line][col] & 0xff);
            }
	}
            fprintf(fid,"\n");
    }

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

// Increment through the registers and save to array
int getData(int row, int startInd)
{
  int col;

  //printf("Starting to get data for row %d...\n",row);

  for (col = startInd; col < startInd + NUM_COLS; col++){
      //printf("Reading data for column %d...\n",col);
      frameData[row][col] = pruDataMem_int[col];
  }

  return (0);
}
