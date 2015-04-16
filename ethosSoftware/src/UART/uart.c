/* file used for the uart functions that will send and receive data from the gnd */

/* system includes needed for the port */
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

/************************************************* 
* Function that starts the uart and opens the port
* It clears all old port settings and resets the 
* port using the new settings.
*************************************************/
void initUart(){
	/* struct for holding the port configurations */
	struct termios newConfig, oldConfig;

	// Uart object to open the file
	FILE *uart;

	// Open the device
	uart = fopen("/sys/devices/bone_capemgr.*/slots", "r+");
	fd = open("/dev/ttyO1", O_RDWR | O_NOCTTY);
	//if(fd < 0)
		/* do nothing for now but maybe later something will go here */

	// Get and store the current attributes of the port
	tcgetattr(fd, &oldConfig);
	// clear the new config for setting it up as desired
	bzero(&newConfig, sizeof(newConfig));

	// set the new settings
	newConfig.c_cflag = B57600 | CS8 | CLOCAL | CREAD;
	newConfig.c_iflag = IGNPAR | ICRNL | IXON | IXOFF;

	newConfig.c_oflag = 0;
	newConfig.c_lflag = 0;

	newConfig.c_cc[VTIME] = 0;
	newConfig.c_cc[VMIN] = 1;

	// Clean and activate the port with the new settings
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newConfig);
}

/************************************************* 
* Function that reads the port line for any data.
* Ff none is found then it will return a 0.
*************************************************/
int readUart(){

	/* create a buffer to hold any incoming data */
	char readbuf[1] = {0};
	int status;

	/* Read data from the port if there is any */
	read(fd,&readbuf, sizeof(readbuf));
	if(status < 0){
		printf("read failed");
	}

	return readbuf[0];

}

/************************************************* 
* Function that writes data to the port that is
* given from the calling function.
*************************************************/
void writeUart(char dataArray[]){
	int status;

	/* write data to the uart port */
	status = write(fd,&dataArray, sizeof(dataArray));
	if(status < 0){
		printf("write failed");
	}
}
