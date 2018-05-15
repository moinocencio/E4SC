#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "pwm.h"


void set_pwm()
{
    T3CONbits.TCKPS = 1; 
    PR3 = 999; 
    TMR3 = 0;
    T3CONbits.TON = 1;
    OC2CONbits.OCM = 6;
    OC2CONbits.OCTSEL = 1;
    OC2RS = ((PR3 + 1) * 50)/100;
    OC2CONbits.ON = 1;   // Enable OC2 module
}
// Change duty_cycle
void set_pwm_dc(uint32_t dcycle)
{
    T3CONbits.TCKPS = 1;
	PR3 = 999; 
	TMR3 = 0; 
	T3CONbits.TON = 1; 
	OC2CONbits.OCM = 6; 
	OC2CONbits.OCTSEL =1;
	OC2RS = ((PR3 + 1) * dcycle)/100;
	OC2CONbits.ON = 1; // Enable OC2 module
}
// Test with different duty-cycle
void testPWM()
{
    uint32_t i=0, j=0;
    int dc[] = {20,40,70,100};
    for(j=0;j<4;j++)
    {
        set_pwm_dc(dc[j]); 
        printf("\ndc = %i\r",dc[j]);
        for(i=0; i<1000000;i++)
        {    
        }
    }
}