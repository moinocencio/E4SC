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

void config_adcInt() // Configuração das Interrupções
{
    // ADC Interrupt
    IPC6bits.AD1IP = 2;     // Priority
	IEC1bits.AD1IE = 1;     // Enable
	IFS1bits.AD1IF = 0;     // Reset AD1IF flag
}

void config_extInt1() // OverCurInt - pin 2
{
    IEC0bits.INT1IE = 0;    
    INTCONbits.INT1EP = 1;  // Rising Edge 
    IPC1bits.INT1IP = 6;    // Priority 
    IEC0bits.INT1IE = 1;    // Enable
    IFS0bits.INT1IF = 0;    // Reset Flag
}

void config_extInt2() // External Interrupt 2 - PhaseATrig - pin 7
{
    IEC0bits.INT2IE = 0;
    INTCONbits.INT2EP = 1;  // Rising Edge
    IPC2bits.INT2IP = 5;    // Priority 
    IEC0bits.INT2IE = 1;    // Enable
    IFS0bits.INT2IF = 0;    // Reset Flag
}

void config_extInt3() // External Interrupt 3 - PhaseBTrig - pin 21
{
    IEC0bits.INT3IE = 0;
    INTCONbits.INT3EP = 1;  // Rising Edge
    IPC3bits.INT3IP = 5;    // Priority 
    IEC0bits.INT3IE = 1;    // Enable
    IFS0bits.INT3IF = 0;    // Reset Flag
}

void config_extInt4() // External Interrupt 4 - PhaseCTrig - pin 20
{
    IEC0bits.INT4IE = 0;
    INTCONbits.INT4EP = 0;  // Falling Edge
    IPC4bits.INT4IP = 5;    // Priority 
    IEC0bits.INT4IE = 1;    // Enable
    IFS0bits.INT4IF = 0;    // Reset Flag
}

void config_Inttimer2() // PWM HIGH
{
    IPC2bits.T2IP = 1;      
    IEC0bits.T2IE = 1;      // Set Timer 2 Interrupt Enable bit 
}

void config_Inttimer3() // PWM LOW
{
    IPC3bits.T3IP = 1;
    IEC0bits.T3IE = 1;      // Set Timer 2 Interrupt Enable bit 
}

void config_Inttimer5() // Sequence Control
{
    IPC5bits.T5IP = 5;      
    IEC0bits.T5IE = 1;      /* Set Timer 5 Interrupt Enable bit */
}
