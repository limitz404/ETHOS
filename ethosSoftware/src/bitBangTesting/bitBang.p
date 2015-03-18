.origin 0
.entrypoint MAIN

// ***************************************
// *      Local Variable Definitions     *
// ***************************************

#define NUMCOLS         162             // number of columns
#define NUMROWS         128             // number of row
#define NUMREGS         12

#define FRAME_VALID             r31.t9
#define LINE_VALID              r31.t11
#define PCLK                    r31.t10
#define PRU1_ARM_INTERRUPT      20
// ***************************************
// *              MAIN CODE              *
// ***************************************


//FRAME_VALID   r31.t9
//LINE_VALID    r31.t11
//PCLK          r31.t10
//DATA          r31.t0-7
//OUT           r31.t1

//data RAM address              r6
//line error counter            r8
//masking register              r5

//line counter                  r14
//inner loop counter            r15
//outer loop counter            r16

MAIN:
        MOV             r14, 0                  // initialize line counter
        MOV             r21, 0                  // initialize address to 0x00000000

REG_LOOP:
        MOV             r6, 4
        MOV             r16,0

ROW_LOOP:

LOOP    COL_LOOP, NUMCOLS                       // hardware assisted loop (LINE)  

        // everything important
        SBBO            r14, r6, 0, 4           // copy data lines to address in r6, use only if masking is done on CPU side, and above three lines are commented out
        ADD             r6, r6, 4               // increment memory location by atomic size

        COL_LOOP:

        ADD             r14, r14, 1                     // increment line counter

        LSL             r20, r16, 8                     // r20 = r16 << 8
        OR              r20, r20, r14                   // r20 |= r14

        SBBO            r20, r21, 0, 4                  // *r21 = r20

        QBEQ            EXIT, r14, NUMROWS

        // CHANGES FOR NO CAMERA OPERATION
        MOV             r10, 1000000
        QBA             DELAY

POSTDELAY:

        // branch back to ROW_LOOP
        ADD             r16, r16, 1
        QBNE            ROW_LOOP, r16, NUMREGS

        // branch back to REG_LOOP if not finished
        QBA             REG_LOOP

DELAY:
        SUB             r10, r10, 1
        QBNE            DELAY, r10, 0
        QBA             POSTDELAY

// ***************************************
// *        TERMINATION FUNCTIONS        *
// ***************************************

EXIT:
        // Send notification to Host for program completion
        MOV             r31.b0, PRU1_ARM_INTERRUPT+16
        // Halt the processor
        HALT
