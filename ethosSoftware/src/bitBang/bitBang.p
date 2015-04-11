.origin 0
.entrypoint MAIN

// ***************************************
// *      Local Variable Definitions     *
// ***************************************

#define NUM_COLS                 162            // number of columns
#define NUM_ROWS                 128            // number of row
#define NUM_REGS                 12

#define FRAME_VALID             r31.t9
#define LINE_VALID              r31.t11
#define PCLK                    r31.t10
#define PRU1_ARM_INTERRUPT      20

// ***************************************
// *              MAIN CODE              *
// ***************************************

//FRAME_VALID               r31.t9
//LINE_VALID                r31.t11
//PCLK                      r31.t10
//DATA                      r31.t0-7
//OUT                       r31.t1

//data RAM address          r6
//line error counter        r8
//masking register          r5

//line counter              r14
//inner loop counter        r15
//outer loop counter        r16

//temp reg for reading ack  r20
//ACK address (0x00)        r21

MAIN:
    MOV     r21, 0                  // initialize ACK address to 0x00000000
    QBA     WAIT

MAINLOOP:
    MOV     r14, 0                  // initialize line counter
    MOV     r6, 4                   // intialize data ram
    MOV     r16, 0                  // intialize outer loop counter
    WBC     FRAME_VALID             // wait until FRAME_VALID is clear
    WBS     FRAME_VALID             // wait until FRAME_VALID is set (rising edge)
    QBA     BITBANG                 // immediately take data    

REG_LOOP:
    MOV     r6, 4                   // intialize data ram
    MOV     r16, 0                  // intialize outer loop counter

ROW_LOOP:
    WBC     LINE_VALID              // wait until FRAME_VALID is clear
    WBS     LINE_VALID              // wait until LINE_VALID is set
    QBA     BITBANG

LOOP    COL_LOOP, NUM_COLS           // hardware assisted loop (COL_LOOP)  
    WBS     PCLK                    // wait until clock is high
    WBC     PCLK                    // wait until clock is low (falling edge)

BITBANG:

    // everything important
    SBBO    r31, r6, 0, 4           // copy data lines to address in r6, use only if masking is done on CPU side, and above three lines are commented out
    ADD     r6, r6, 4               // increment memory location by atomic size

COL_LOOP:
    ADD     r14, r14, 1             // increment line counter
    LSL     r20, r16, 8             // r20 = r16 << 8
    OR      r20, r20, r14           // r20 |= r14
    SBBO    r20, r21, 0, 4          // *r21 = r20
    QBEQ    WAIT, r14, NUM_ROWS

    // branch back to ROW_LOOP
    ADD     r16, r16, 1             // increment outer loop counter
    QBNE    ROW_LOOP, r16, NUM_REGS  // loop NUM_REGS times

    // branch back to REG_LOOP if not finished
    QBA     REG_LOOP

// ***************************************
// *        TERMINATION FUNCTIONS        *
// ***************************************

WAIT:
    LBBO    r20, r21, 0, 4
    QBNE    MAINLOOP, r20, r1
    QBA     WAIT
