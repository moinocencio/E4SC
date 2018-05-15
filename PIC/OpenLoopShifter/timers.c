#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "timers.h"

// PWM
void config_timer2()
{
    T2CON = 0; 
    T2CONbits.ON = 0; 
    T2CONbits.TCKPS = 1;        // Prescaler = 2 -> f = 20 MHz
    TMR2 = 0;
    PR2 = 124; 
    T2CONbits.TON = 1; 
}

// Phase Shifter Timer
void config_timer5()            
{
    T5CON = 0;                  // Clear all registers 
    T5CONbits.ON = 0;           // Stop the timer 
    T5CONbits.TCKPS = 7;        // 
    TMR5 = 0;
    PR5 = 250; 
    T5CONbits.TON = 1; 
}