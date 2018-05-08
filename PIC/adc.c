#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "adc.h"

void config_adc()
{
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
}