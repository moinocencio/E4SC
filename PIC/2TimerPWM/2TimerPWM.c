#define PBCLK 40000000

#include "config_bits.h"
#include <stdint.h>
#include <xc.h>            
#include <sys/attribs.h> 
#include <stdio.h>
#include <stdlib.h>
#include "timers.h"
#include "int.h"

/* 2 Timer PWM:
 * Implement the PWM signal with 2 timers.
 * One controls the frequency, the other controls the duty cycle.
 */

void main (void) {
    /* Set PWM pins */
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    /* Set frequency timer (2) */
    config_timer2();
    /* Set duty cycle Timer  (5)*/
    config_timer5();
    /* Set Interruptions */
    config_Inttimer2();
    config_Inttimer5();
    TMR5 = 0;
    TMR2 = 0;
    enableInterrupts();
    
    while(1);
}

void __ISR(8) isr_timer2(void){  // Put outputs to HIGH and enable T5
    LATE |= !0xFFC0;
    TMR5 = 0;
    TMR2 = 0;
    /* Set Timer 5 Interrupt Enable bit  */
    IEC0bits.T5IE = 1;     
    /* Reset flag */
    IFS0bits.T5IF = 0;
    IFS0bits.T2IF = 0;
}

void __ISR(20) isr_timer5(void){ // Put outputs to LOW and disable T5
    LATE &= 0xFFC0;
    TMR5 = 0;
    /* Disable Timer 5 Interrupt */
    IEC0bits.T5IE = 0;  
    /* Reset flag */
    IFS0bits.T5IF = 0;
}