
#include "config_bits.h"
#include <xc.h>


void set_pwm(void)
{
    T3CONbits.TCKPS = 2;
	PR3 = 512; 
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
	// timer configuration sequence)
	OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC1CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC1RS = 128; // Ton constant
	OC1CONbits.ON = 1; // Enable OC1 module
}
void main (void)
{
    set_pwm();
    while(1);
}