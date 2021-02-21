/*
 * File:   master.c
 * Author: Dina
 *
 * Created on February 12, 2021, 4:06 PM
 */
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//Librerías
 //ya existentes
#include <xc.h>
#include <stdint.h> 
//creados
#include "SPI.h"
#include "USART.h"
#include "LCD.h"
#include "OSCILADOR.h"
//
#define _XTAL_FREQ 8000000   //PAra el crystal

//variables

uint8_t ADC1,ADC12,ADC13;
uint8_t CONTADOR,CONT,CONT1,CONT2,CONT3;
uint8_t TEMP,TEMP1,TEMP2;
uint8_t TEM,TEM1,TEM2;
uint8_t SND, TGLTX;
uint8_t VADC;

void setup(void);
void RESET(void);
void SLAVE1(void);
void SLAVE2(void);
void SLAVE3(void);
void SEND(void);
void INFCONT(void);
void INFADC(void);
void INFADC(void);
void CTRLTEMP(void);
const char* STRINGCONT(char C1, char C2, char C3);
const char* STRINGADC(char C1, char C2, char C3);
const char* STRINGTEMPP(char C1, char C2, char C3);
const char* STRINGTEMPN(char C1, char C2, char C3);


void __interrupt() isr(void){
    
    if(PIR1bits.TXIF == 1){
        SEND();
        SND++;
        PIE1bits.TXIE = 0;
        PIR1bits.TXIF = 0;
    }
}

void main(void) {
    initOSC(8);    //LLAMO CONFIGURACIÓN PARA EL OSCILADOR INTERNO
    CONFTXR();    //CONFIG DE COMUNICACIÓN USART
    CONFRXT();
    setup();
    SPIinit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE); //CONFIG DE SPI COMO MASTER
    LCDinit();   
    LCDCmd(0x8A);
    LCDGoto(1,1);
    LCDPrint("VOLT");
    LCDGoto(7,1);
    LCDPrint("CONT");
    LCDGoto(13,1);
    LCDPrint("TEMP");     
    while(1){
        TGLTX++; //CONTADOR PARA DELAY DEL TX
        if (TGLTX > 2){ //DELAY DEL TX PARA QUE MANDE INFORMACIÓN CORRECTAMENTE
            PIE1bits.TXIE = 1;
            TGLTX = 0;  }      
        SLAVE1(); //LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVEADC
        INFADC();//LLAMO FUNCION DE MAPEO
        SLAVE2();//LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVECONT
        INFCONT();//FUNCION DE MAPEO
        SLAVETEMP();//LAMO RUTINA DE QUE SIRVE PARA RECIBIR DATOS DEL SLAVETEMP
        LCDGoto(2,2);
        LCDPrint(STRINGADC(ADC1, ADC12, ADC13));
        LCDGoto(8,2);
        LCDPrint(STRINGCONT(CONT1, CONT2, CONT3));
        LCDGoto(13,2);
        CTRLTEMP();     //FUNCION PARA DETERMINAR SIGNO POSITIVO Y NEGATIVO
    }
}
void RESET(){   
    PORTA = 0;//LIMPIEZA DE PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0b00010000;
    TRISD = 0;
    TRISE = 0; 
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1; 
    PIR1bits.TXIF = 0;
    PIE1bits.TXIE = 1; 
}
    
void Setup(){
    RESET();
}

void Slave1(void){//FUNCIÓN QUE SIRVE PARA RECIBIR LOS DATOS DEL SPI ESCLAVO
    PORTAbits.RA0 = 0;
    __delay_ms(1);
    
    SPIWrite(CLN);
    VADC = SPIRead();
    
    __delay_ms(2);
    PORTAbits.RA0 = 1;
}

void Slave2(void){
    PORTAbits.RA1 = 0;
    __delay_ms(2);
    
    SPIWrite(CLN);
    CONTADOR = SPIRead();
    
    __delay_ms(2);
    PORTAbits.RA1 = 1;    
}

void Slave3(void){
    PORTAbits.RA2 = 0;
    __delay_ms(2);
    
    SPIWrite(CLN);
    TEMP = SPIRead();
    
    __delay_ms(2);
    PORTAbits.RA2 = 1;   
}


void INFCONT(void){         //CONTADOR
    
    CONT1 = CONTADOR/100;
    CONT2 = ((CONTADOR-(CONT1*100))/10);
    CONT3 = (CONTADOR-(CONT1*100))-(CONT2*10);
    CONT1 = CONT1 + 0x30;
    CONT2 = CONT2 + 0x30;
    CONT3 = CONT3 + 0x30;
}

void INFADC(void){      //ADC
    
    ADC1 = VADC/51;
    ADC12 = (VADC-(ADC1*51))/10;
    ADC13 = (VADC-(ADC1*51))-(ADC12*10);
    ADC1 = ADC1+0x30;
    ADC12 = ADC12+0x30;
    ADC13 = ADC13+0x30;
}

void INFTEMPP(void){        //TEMPERATURA
    
    TEMP1 = ((TEMP-68)*150)/187;
    TEM1 = TEMP1/100;
    TEM2 = (TEMP1-(TEM1*100))/10;
    TEM3 = (TEMP1-(TEM1*100)-(TEMP2*10));
    TEM1 = TEM1+0x30;
    TEM2 = TEM2+0x30;
    TEM3 = TEM3+0x30; 
}
void INFTEMPN(void){            //NEGATIVO TEMPERATURA
    TEMP2 = ((TEMP*(-55))/68)+55;
    TEM1 = TEMP2/100;
    TEM2 = (TEMP2-(TEM1*100))/10;
    TEM3 = (TEMP2-(TEM1*100)-(TEM2*10));
    TEM1 = TEM1+0x30;
    TEM2 = TEM2+0x30;
    TEM3 = TEM3+0x30;
}

void CTRLTEMP(void){            //AGREGA EL SIGNO + -
    if(TEMP >= 68){ //SI LA TEMPERATURA ES MAYOR O IGUAL A 68
        INFTEMPP();
        LCDPrint(STRINGTEMPP(TEM1, TEM2, TEM3));  
    }
    else if(TEMP < 68){
        INFTEMPN();
        LCDPrint(STRINGTEMPN(TEM1, TEM2, TEM3));
    }
}

const char* STRINGADC(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = C1;
    TEMP[1] = 0x2E;    
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}


const char* STRINGCONT(char C1, char C2, char C3){ 
    char TEMP[3];
    TEMP[0] = C1;
    TEMP[1] = C2;
    TEMP[2] = C3;
    return TEMP;
}


const char* STRINGTEMPP(char C1, char C2, char C3){
    char TEMP[4];
    TEMP[0] = 0x2B;
    TEMP[1] = C1;   
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

const char* STRINGTEMPN(char C1, char C2, char C3){
    char TEMP[5];
    TEMP[0] = 0x2D;
    TEMP[1] = C1;   
    TEMP[2] = C2;
    TEMP[3] = C3;
    return TEMP;
}

void SEND(void){
    
    switch(SND){
        case 0:
            TXREG = 0x28;
            break;
        case 1:
            TXREG = ADC1;
            break;
        case 2:
            TXREG = 0x2E;
            break;    
        case 3:
            TXREG = ADC12;
            break;    
        case 4:
            TXREG = ADC13;
            break;    
        case 5:
            TXREG = 0x29;
            break;    
        case 6:
            TXREG = 0x2C;
            break;    
        case 7:
            TXREG = 0x28;
            break;
        case 8:
            TXREG = CONT1;
            break;    
        case 9:
            TXREG = CONT2;
            break;    
        case 10:
            TXREG = CONT3;
            break;       
        case 11:
            TXREG = 0x29;
            break;       
        case 12:
            TXREG = 0x2C;
            break;        
        case 13:
            TXREG = 0x28;
            break;      
        case 14:
            if(TEMP >= 68){
             TXREG = 0x2B;   
            }else if (TEMP < 68){
             TXREG = 0x2D;   
            }
            break;            
        case 15:
            TXREG = TEM1;
            break;
        case 16:
            TXREG = TEM2;
            break;    
        case 17:
            TXREG = TEM3;
            break;            
        case 18:
            TXREG = 0x29;
            break;       
        case 19:
            TXREG = 0x0D;
            SND = 0;
            break;
    }
}
