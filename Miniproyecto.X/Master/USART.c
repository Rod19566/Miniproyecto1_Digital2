/*
 * File:   USART.c
 * Author: Dina
 *
 * Created on February 18, 2021, 9:06 PM
 */


#include <xc.h>
#include <stdint.h>
//LIBRERIA CREADA
#include "USART.h"


void CONFTXR(void);
void CONFRXT(void);
void PRINT(char *VAL); //LECTURA


void CONFTXR(void){
    TXSTAbits.BRGH = 1;
    TXSTAbits.TX9 = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.TXEN = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;
}

void CONFRXT(void){
    RCSTAbits.OERR = 0;
    RCSTAbits.SPEN = 1; 
    RCSTAbits.CREN = 1;
    PIE1bits.RCIE = 1;
    RCSTAbits.FERR = 0;
    RCSTAbits.RX9 = 0;
}

void PRINT(char *VAL){
    TXREG = VAL;
    TXREG = 0x2E;
}
