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
// OverCurInt - pin 2
void config_extInt1()
{
    // INT0 ? External Interrupt 0 3 3 
    // Flag - IFS0<3>
    // Enable - IEC0<3>
    // Priority - IPC0<28:26>
    // Sub-Priority IPC0<25:24>
    IEC0bits.INT1IE = 0;
    INTCONbits.INT1EP = 1; // Para ser activo a 1
    IPC1bits.INT1IP = 3; // Priority 
    IEC0bits.INT1IE = 1; // Enable
    IFS0bits.INT1IF = 0; // Reset Flag
}
// External Interrupt 2 - PhaseATrig - pin 7
void config_extInt2()
{
    IEC0bits.INT2IE = 0;
    INTCONbits.INT2EP = 1; // Para ser activo a 1
    IPC2bits.INT2IP = 3; // Priority 
    IEC0bits.INT2IE = 1; // Enable
    IFS0bits.INT2IF = 0; // Reset Flag
}
// External Interrupt 3 - PhaseBTrig - pin 21
void config_extInt3()
{
    IEC0bits.INT3IE = 0;
    INTCONbits.INT3EP = 1; // Para ser activo a 1
    IPC3bits.INT3IP = 3; // Priority 
    IEC0bits.INT3IE = 1; // Enable
    IFS0bits.INT3IF = 0; // Reset Flag
}
// External Interrupt 4 - PhaseCTrig - pin 20
void config_extInt4()
{
    IEC0bits.INT4IE = 0;
    INTCONbits.INT4EP = 0; // Para ser activo a 1
    IPC4bits.INT4IP = 3; // Priority 
    IEC0bits.INT4IE = 1; // Enable
    IFS0bits.INT4IF = 0; // Reset Flag
}
// PWM
void config_Inttimer2()
{
    IPC2bits.T2IP = 3;      /* Timer 2 priority: 2 */
    IPC2bits.T2IS = 0;      /* Timer 2 sub-priority: 0 */   
    
    // IPC2bits.OC2IP = 2;     /* Output Compare 2 priority: 2 */
    // IPC2bits.OC2IS = 0;     /* Output Compare 2 sub-priority: 0 */

    IEC0bits.T2IE = 1;      /* Set Timer 2 Interrupt Enable bit */
    // IEC0bits.OC2IE = 1;     /* Set Output Compare 2 Interrupt Enable bit */
}
// Duty_Cycle
void config_Inttimer5()
{
    IPC5bits.T5IP = 3;      /* Timer 5 priority: 2 */
    IPC5bits.T5IS = 0;      /* Timer 5 sub-priority: 0 */
    IEC0bits.T5IE = 1;      /* Set Timer 5 Interrupt Enable bit */
}
