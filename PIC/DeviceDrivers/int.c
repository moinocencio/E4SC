#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h> 

#include "int.h"

// Enable System Interrupts
void enableInterrupts()
{
    // Possivelmente trocar estas duas linhas pode ajudar 
    INTCONSET = _INTCON_MVEC_MASK;    /* Set the interrupt controller for multi-vector mode */
    printf("\nprint1\r");   
    __builtin_enable_interrupts(); 
    printf("\nprint2\r");
}
// Configuração das Interrupções
void config_adcInt()
{
    // Interrupção da ADC
    IPC6bits.AD1IP = 2;
	IEC1bits.AD1IE = 1;
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
}
void config_extInt0()
{
    // INT0 ? External Interrupt 0 3 3 
    // Flag - IFS0<3>
    // Enable - IEC0<3>
    // Priority - IPC0<28:26>
    // Sub-Priority IPC0<25:24>
    IEC0bits.INT0IE = 0;
    INTCONbits.INT0EP = 1; // Para ser activo a 1
    IPC0bits.INT0IP = 3; // Priority 
    IEC0bits.INT0IE = 1; // Enable
    IFS0bits.INT0IF = 0; // Reset Flag
}
void config_extInt1()
{
    // External Interrupt 1 - PhaseATrig
    IEC0bits.INT1IE = 0;
    INTCONbits.INT1EP = 1; // Para ser activo a 1
    IPC1bits.INT1IP = 3; // Priority 
    IEC0bits.INT1IE = 1; // Enable
    IFS0bits.INT1IF = 0; // Reset Flag
}
void config_extInt2()
{
    // External Interrupt 2 - PhaseBTrig
    IEC0bits.INT2IE = 0;
    INTCONbits.INT2EP = 1; // Para ser activo a 1
    IPC2bits.INT2IP = 3; // Priority 
    IEC0bits.INT2IE = 1; // Enable
    IFS0bits.INT2IF = 0; // Reset Flag
}
void config_extInt3()
{
    // External Interrupt 3 - PhaseCTrig
    IEC0bits.INT3IE = 0;
    INTCONbits.INT3EP = 1; // Para ser activo a 1
    IPC3bits.INT3IP = 3; // Priority 
    IEC0bits.INT3IE = 1; // Enable
    IFS0bits.INT3IF = 0; // Reset Flag
}
