
#include <stdio.h>
#include <stdlib.h>
#include "config_bits.h"
#include <xc.h>

void config_uart()
{
    U1MODEbits.BRGH = 0; // 9600 baudrate
    U1BRG = 256; //257;
    
    U1MODEbits.SIDL = 0;
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 0;
    U1MODEbits.UEN = 0b00;
    U1MODEbits.WAKE = 1;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.RXINV = 0;
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;
    U1MODEbits.ON = 1;
    
    U1STAbits.UTXINV = 0;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
    
    
}

void main (void)
{
    config_uart();
    __XC_UART=1;
    float res=0;

     // Generic config.
     AD1CON1bits.SSRC=7; // Internal counter ends sampling and starts conversion
     AD1CON1bits.CLRASAM=1; //Stop conversion when 1st A/D converter interrupt is generated an
     AD1CON1bits.FORM=0; // Integer 16 bit output format
     AD1CON2bits.VCFG=0; // VR+=AVdd; VR-=AVss
     AD1CON2bits.SMPI=0; // Number (+1) of consecutive coversions, stored in buffer
     AD1CON3bits.ADRC=1; // ADC uses internal RC clock
     AD1CON3bits.SAMC=16; // Sample time is 16TAD ( TAD = 100ns)
     
        // Set AN0 as input
    AD1CHSbits.CH0SA=0; // Select AN0 as input for A/D converter
    TRISBbits.TRISB0=1; // AN0 in input mode
    AD1PCFGbits.PCFG0=0; // AN0 as analog input
    // Enable module
    AD1CON1bits.ON = 1; // Enable A/D module (Must be the ***last instruction of config phase***)

    while(1) 
    {
       // Get one sample
       IFS1bits.AD1IF = 0; // Reset interrupt flag
       AD1CON1bits.ASAM=1; // Start conversion
       while(IFS1bits.AD1IF == 0); // Wait for EOC
       // Convert to 0..3.3V
       res = (ADC1BUF0 * 3.3) / 1023;
       // Output result
       printf("Voltage: %f\n\r",res);
    }
     
}