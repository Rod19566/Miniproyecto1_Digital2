/*
 * File:   SPI.c
 * Author: Dina
 *
 * Created on February 18, 2021, 11:45 PM
 */


#include <xc.h>
//
#include "SPI.h"

unsigned SPIDR() 
{
    if(SSPSTATbits.BF){
        return 1;
    }
    else{
        return 0;
    }
}

char SPIRead()
{
    SPIRW();        
    return(SSPBUF); 
}

void SPIinit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) 
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else             
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void SPIRW()
{
    while ( !SSPSTATbits.BF ); 
}

void SPIWrite(char dat)  
{
    SSPBUF = dat;
}
