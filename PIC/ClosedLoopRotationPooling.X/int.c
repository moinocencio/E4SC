#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h> 
#include "int.h"

void enableInterrupts() // Enable System Interrupts
{
 
    // Problem 
    INTCONSET = _INTCON_MVEC_MASK;    
    // INTCONbits.MVEC = 1;
    __builtin_enable_interrupts(); 
}

void disableInterrupts() // Disable System Interrupts
{
    __builtin_disable_interrupts(); 
}

void config_Inttimer2() // PWM HIGH
{
    IPC2bits.T2IP = 1;      
    //IEC0bits.T2IE = 1;      // Set Timer 2 Interrupt Enable bit 
}

void config_Inttimer3() // PWM LOW
{
    IPC3bits.T3IP = 1;
    //IEC0bits.T3IE = 1;      // Set Timer 2 Interrupt Enable bit 
}

void config_Inttimer5() // Sequence Control
{
    IPC5bits.T5IP = 5;      
    //IEC0bits.T5IE = 1;      /* Set Timer 5 Interrupt Enable bit */
}

void config_U1_int()
{
    // IPC6bits.U1IS = ;    // Sub-priority
    IPC6bits.U1IP = 3;      // Priority
    IEC0bits.U1TXIE = 0;    // Enable 
    IEC0bits.U1RXIE = 1;    // Enable   
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
}
