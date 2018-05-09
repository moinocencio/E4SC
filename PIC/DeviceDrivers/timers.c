#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "timers.h"

volatile freq = 0; 

int get_freq()
{
    return freq;
}
// Frequência do PWM
void config_timer2(int f)
{
    freq = f;
    T2CON = 0;              /* Clear all registers */
    T2CONbits.ON = 0;       /* Stop the timer */
    T2CONbits.TCKPS = 5; // 32 , f = 1250000 Hz
    PR2 = ((40000000/(2^T2CONbits.TCKPS))/freq)-1;   // 20 kHz
    TMR2 = 0;
    T2CONbits.TON = 1; 
}
// Duty_Cycle
void config_timer5(int duty_cycle)
{
    T5CON = 0;              /* Clear all registers */
    T5CONbits.ON = 0;       /* Stop the timer */
    T5CONbits.TCKPS = 5; // 32 , f = 1250000 Hz
    PR5 = ((40000000/(2^T5CONbits.TCKPS))/(freq*(1/(duty_cycle/100))))-1;   // 20 kHz
    TMR5 = 0;
    T5CONbits.TON = 1; 
}
