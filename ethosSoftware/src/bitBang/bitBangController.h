/****************************************************************************
* AUTHORS: PATRICK KLEIN, TAYLOR DEAN                                        *
* TEAM: ETHOS                                                                *
* FILE: bitBangController.c                                                  *
* CREATED: 3 MARCH 2015                                                      *
* PURPOSE: TO LOAD BITBANG ASSEMBLY CODE ONTO PRU 1, START THE CODE, THEN    *
*          GRAB IMAGE DATA FROM THE PRU DATA RAM.                            *
*****************************************************************************/
#ifndef BITBANG_H
#define BITBANG_H

#define NUMCOLS 162

int initializePRU( void );
int getImage( int (*)[NUMCOLS] );
void closePRU( void );

#endif
