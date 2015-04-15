
/*****************************************************************************
* AUTHORS: PATRICK KLEIN, TAYLOR DEAN                                        *
* TEAM: ETHOS                                                                *
* FILE:                                                   *
* CREATED: 2015                                                      *
* PURPOSE:     *
*                                      *
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


/*****************************************************************************
* Local Macro Declarations                                                   *
*****************************************************************************/

extern "C" {
    #include "bitBangController.h"
}


/*****************************************************************************
* Global Variable Definitions                                                *
*****************************************************************************/

using namespace std;

/*****************************************************************************
* Global Function Definitions                                                *
*****************************************************************************/


void canHandler( struct attitude *displacement; float *health ){

    while( 1 ){

        flag = canDump();

        if( flag & 0b001 ){
            canSend(attitude);    // SEND ATTITUDE
        }

        if( flag & 0b010 ){
            canSend(health);      // SEND HEALTH DATA
        }

        if( flag & 0b100 ){
            attitude oldFlag;
            ret = parseLog(&logAtt);
            if( !ret ){
                canSend(logAtt);  // SEND OLD LOG DATA
            } else {
                canSend(NULL);    // SEND NULL OR NOTHING
            }
        }

        if( !(flag & 0b111) ){
            return(0);
        }

    }

}
