#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "timers.h"

// PWM HIGH
void config_timer2()
{
    T2CON = 0; 
    T2CONbits.ON = 0; 
    T2CONbits.TCKPS = 0;        
    TMR2 = 0;
    PR2 = 1999; 
    T2CONbits.TON = 1;   
}

// PWM LOW
void config_timer3()            
{
    T3CON = 0;                  // Clear all registers 
    T3CONbits.ON = 0;           // Stop the timer 
    T3CONbits.TCKPS = 0;
    TMR3 = 0;
    PR3 = 199; 
    T3CONbits.TON = 1; 
}

// Phase Shifter Timer
void config_timer5()            
{
    T5CON = 0;                  // Clear all registers 
    T5CONbits.ON = 0;           // Stop the timer 
    T5CONbits.TCKPS = 6;         
    TMR5 = 0;
    PR5 = 600; 
    T5CONbits.TON = 1; 
}
