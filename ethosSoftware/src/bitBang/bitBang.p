.origin 0
.entrypoint MAIN

#include "bitBang.hp"

// ***************************************
// *      Local Variable Definitions     *
// ***************************************

#define GPIO1				0x4804c000
#define GPIO_CLEARDATAOUT	0x190
#define GPIO_SETDATAOUT		0x194

#define MASK		0xff

#define IMGRAM		0x00010004

#define NUMCOLS		128			// number of columns
#define NUMROWS		162			// number of row

#define FRAME_VALID	r31.t9
#define LINE_VALID	r31.t11
#define	PCLK		r31.t10
#define OUT		r31.t1

#define TEST		0xff

// ***************************************
// *       Local Macro Definitions       *
// ***************************************

// Affects:  r0, r1
.macro PRU_INIT

    // Configure the block index register for PRU0 by setting c24_blk_index[7:0] and
    // c25_blk_index[7:0] field to 0x00 and 0x00, respectively.  This will make C24 point
    // to 0x00000000 (PRU0 DRAM) and C25 point to 0x00002000 (PRU1 DRAM).
    MOV       r0, 0x00000000
    MOV       r1, CTBIR_1
    ST32      r0, r1

.endm

// ***************************************
// *              MAIN CODE              *
// ***************************************

/*
FRAME_VALID	r31.t9
LINE_VALID	r31.t11
PCLK		r31.t10
DATA		r31.t0-7
OUT		r31.t1
*/

/*
data RAM address		r6
line error counter		r8
masking register		r5

line counter			r14
inner loop counter		r15
outer loop counter		r16
*/


MAIN:
		
		
		PRU_INIT

		MOV		r14, 0				// initialize line counter
		MOV 		r8, 0				// initialize line error counter

		WBC		FRAME_VALID			// wait until FRAME_VALID is clear
		WBS		FRAME_VALID			// wait until FRAME_VALID is set (rising edge)

		

OUTERLOOP:
		MOV	r6, 0
	
INNERLOOP:

		WBC		LINE_VALID
		WBS		LINE_VALID			// wait until LINE_VALID is set

LOOP		LINELOOP, NUMCOLS				// hardware assisted loop (LINE)  

		// wait until luma byte  is valid
		WBS		PCLK				// wait until clock is high
		WBC		PCLK				// wait until clock is low (falling edge)
		
		// everything important
		//MOV		r5, r31				// copy the data into r5
		//AND		r5, r5, MASK			// mask the data
		//SBBO		r5, r6, 0, 4 			// copy r5 to the address in r6
		SBBO		r31,r6, 0, 4			// copy data lines to address in r6, use only if masking is done on CPU side, and above three lines are commented out
		ADD		r6, r6, 4			// increment memory location by atomic size
		
		// end of line check after luma byte
		//QBBC		LINE_ERR,LINE_VALID		// EXPERIMENTAL, branching to hardware-assisted loop

		// ignore chroma byte
		WBS		PCLK				// wait until clock is high
		ADD		r14, r14, 1			// increment line counter
		WBC		PCLK				// wait until clock is low (falling edge)

		// end of line check after chroma byte
		//QBBC		LINE_ERR,LINE_VALID		//EXPERIMENTAL, branching to hardware-assisted loop
		
	LINELOOP:
		
		// branch back to INNERLOOP
		MOV	r31.b0, PRU1_ARM_INTERRUPT+16
		ADD	r16, r16, 1
		QBNE	INNERLOOP, r16, 15

		// branch back to OUTERLOOP if not finished
		QBNE	OUTERLOOP, r14, 162


		//QBNE		ERR_EXIT,r8,0
		JMP		EXIT		// EXIT program

LINE_ERR:
		ADD		r8,r8,1
		JMP		ERR_EXIT
		
		

// ***************************************
// *        TERMINATION FUNCTIONS        *
// ***************************************


EXIT:
    	// Send notification to Host for program completion
    	MOV		r31.b0, PRU1_ARM_INTERRUPT+16

    	// Halt the processor
	HALT
    
ERR_EXIT:
	// Send notification to Host for line error	
	MOV		r31.b0, PRU1_ARM_INTERRUPT+16

	//Halt the processor
	HALT
