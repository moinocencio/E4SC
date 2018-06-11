#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h> 

#include "int.h"

// Enable System Interrupts
void enableInterrupts()
{
    // INTCONSET = _INTCON_MVEC_MASK;    
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts(); 
}
// Disable System Interrupts
void disableInterrupts()
{
    __builtin_disable_interrupts(); 
}
// Configuração das Interrupções
void config_adcInt()
{
    // ADC Interrupt
    IPC6bits.AD1IP = 3;     // Priority
	IEC1bits.AD1IE = 1;     // Enable
	IFS1bits.AD1IF = 0;     // Reset AD1IF flag  
}
// OverCurInt - pin 2
void config_extInt1()
{
    IEC0bits.INT1IE = 0;    
    INTCONbits.INT1EP = 1;  // Rising Edge 
    IPC1bits.INT1IP = 3;    // Priority 
    IEC0bits.INT1IE = 1;    // Enable
    IFS0bits.INT1IF = 0;    // Reset Flag
}
// External Interrupt 2 - PhaseATrig - pin 7
void config_extInt2()
{
    IEC0bits.INT2IE = 0;
    INTCONbits.INT2EP = 1;  // Rising Edge
    IPC2bits.INT2IP = 3;    // Priority 
    IEC0bits.INT2IE = 1;    // Enable
    IFS0bits.INT2IF = 0;    // Reset Flag
}
// External Interrupt 3 - PhaseBTrig - pin 21
void config_extInt3()
{
    IEC0bits.INT3IE = 0;
    INTCONbits.INT3EP = 1;  // Rising Edge
    IPC3bits.INT3IP = 3;    // Priority 
    IEC0bits.INT3IE = 1;    // Enable
    IFS0bits.INT3IF = 0;    // Reset Flag
}
// External Interrupt 4 - PhaseCTrig - pin 20
void config_extInt4()
{
    IEC0bits.INT4IE = 0;
    INTCONbits.INT4EP = 0;  // Falling Edge
    IPC4bits.INT4IP = 3;    // Priority 
    IEC0bits.INT4IE = 1;    // Enable
    IFS0bits.INT4IF = 0;    // Reset Flag
}
// PWM
void config_Inttimer2()
{
    IPC2bits.T2IP = 3;      // Timer 2 priority: 2 
    IPC2bits.T2IS = 0;      // Timer 2 sub-priority: 0    
    IEC0bits.T2IE = 1;      // Set Timer 2 Interrupt Enable bit 
    // IEC0bits.OC2IE = 1;  // Set Output Compare 2 Interrupt Enable bit 
}
// Sequence Control
void config_Inttimer5()
{
    IPC5bits.T5IP = 3;      /* Timer 5 priority: 2 */
    IPC5bits.T5IS = 0;      /* Timer 5 sub-priority: 0 */
    IEC0bits.T5IE = 0;      /* Set Timer 5 Interrupt Enable bit */
}
