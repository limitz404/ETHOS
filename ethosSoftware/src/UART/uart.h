/* Header file that holds any constants and function declerations */

#ifndef UART_H
#define UART_H

/* Global Constants */
int fd; // Used for keeping track of the open port

/* Function Delarations */

/************************************************* 
* Function that starts the uart and opens the port
* It clears all old port settings and resets the 
* port using the new settings.
*************************************************/
void initUart();

/************************************************* 
* Function that reads the port line for any data.
* Ff none is found then it will return a 0.
*************************************************/
int readUart();

/************************************************* 
* Function that writes data to the port that is
* given from the calling function.
*************************************************/
void writeUart(char dataArray[]);




void uartHandler( struct attitude *displacement ,  float *health );

#endif
