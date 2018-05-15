#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "adc.h"

volatile float res = 0; 
volatile float value = 0; 

void config_adc()
{
    AD1CON1bits.SSRC=7;     // Internal counter ends sampling and starts conversion
    AD1CON1bits.CLRASAM=1;  // Stop conversion when 1st A/D converter interrupt is generated an
    AD1CON1bits.FORM=0;     // Integer 16 bit output format
    AD1CON2bits.VCFG=0;     // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI=7;     // Number (+1) of consecutive conversions, stored in buffer
    AD1CON3bits.ADRC=1;     // ADC uses internal RC clock
    AD1CON3bits.SAMC=16;    // Sample time is 16TAD ( TAD = 100ns)
    
    TRISBbits.TRISB0=1;     // AN0 in input mode
    TRISBbits.TRISB1=1;     // AN1 in input mode
    
    AD1PCFGbits.PCFG0=0;    // AN0 as analog input
    AD1PCFGbits.PCFG1=0;    // AN1 as analog input 
    
    AD1CON1bits.ON = 1;     // Enable A/D module
}

float readADC(int ADCnr)
{      
    switch(ADCnr)
    {
        case 0: 
            // 
            AD1CHSbits.CH0SA=0; // Select AN0 as input for A/D converter
            break;
        case 1:
            AD1CHSbits.CH0SA=1; // Select AN1 as input for A/D converter  
            break;
        default:
            AD1CHSbits.CH0SA=0; // Select AN0 as input for A/D converter
            break;
    }
    // Get Sample ADC - Polling
    IFS1bits.AD1IF = 0;         // Reset interrupt flag 
    AD1CON1bits.ASAM = 1;       // Start Conversion
    int j =0;
    float aux = 0, val = 0;
	int *p = (int* ) (&ADC1BUF0);
    while(IFS1bits.AD1IF == 0);     // Wait for end of conversion
	for(j = 0; j < 8; j++)
    {
		aux = p[j*4];
        val += (aux*3.3+12)/1023;   //  -> 12 - Calibration
	}
    res = val/8;
    return res;
}
void testADC()
{
    value = readADC(0);
    printf("\nBatVoltMeas_A0: %f\n\r", value);
    delay();
    value = readADC(1);
    printf("\n_MeanCur_A1: %f\n\r", value);
    delay();
}
