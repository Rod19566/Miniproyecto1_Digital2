/*
 * File:   SPI.c
 * Author: Dina
 *
 * Created on February 17, 2021, 8:59 PM
 */

//lIBRERÍA EXISTENTE
#include <xc.h>
//LIBRERIA CREADA
#include "SPI.h"

void SPIinit(Spi_Type, Spi_Data_Sample, Spi_Clock_Idle, Spi_Transmit_Edge);
char SPIRead();        //LECTURA
unsigned SPIDR();       //DATA
static void SPIRW();     //
void SPIWrite(char);    //IMPRESIÓN

void SPIinit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void SPIRW()          // wait FOR all bits
{
    while ( !SSPSTATbits.BF ); // COMPLETION OF DATA
}

void SPIWrite(char dat)       //SPI GETS WRITTEN IN
{
    SSPBUF = dat;
}

unsigned SPIDR()             //IS IT READY TO BE READ?
{
    if(SSPSTATbits.BF){         //RETURNS TRUE (1) IF IT IS
        return 1;
    }
    else{
        return 0;
    }
}

char SPIRead() //READS
{
    SPIRW();       
    return(SSPBUF);             //DATA BUFFER
}
