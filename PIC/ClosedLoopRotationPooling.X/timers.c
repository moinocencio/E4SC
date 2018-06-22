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
    T2CONbits.TCKPS = 1;        
    TMR2 = 0;
    PR2 = 10349; 
    T2CONbits.TON = 1;   
}

// PWM LOW
void config_timer3()            
{
    T3CON = 0;                  // Clear all registers 
    T3CONbits.ON = 0;           // Stop the timer 
    T3CONbits.TCKPS = 1;
    TMR3 = 0;
    PR3 = 349; 
    T3CONbits.TON = 1; 
}

void config_timer5()            
{
    T5CON = 0;                  // Clear all registers 
    T5CONbits.ON = 0;           // Stop the timer 
    T5CONbits.TCKPS = 5;         
    TMR5 = 0;
    PR5 = 14999;
    //PR5 = 0x6D; 
    T5CONbits.TON = 1; 
}