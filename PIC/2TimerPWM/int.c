#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h> 

#include "int.h"

// Enable System Interrupts
void enableInterrupts()
{
 
    // Problem 
    INTCONSET = _INTCON_MVEC_MASK;    
    // INTCONbits.MVEC = 1;
    __builtin_enable_interrupts(); 
}
// Disable System Interrupts
void disableInterrupts()
{
    __builtin_disable_interrupts(); 
}
// PWM
void config_Inttimer2()
{
    IPC2bits.T2IP = 1;      /* Timer 2 priority: 2 */
    IPC2bits.T2IS = 0;      /* Timer 2 sub-priority: 0 */   
    IEC0bits.T2IE = 1;      /* Set Timer 2 Interrupt Enable bit */
}
// Sequence Control
void config_Inttimer5()
{
    IPC5bits.T5IP = 5;      /* Timer 5 priority: 2 */
    IPC5bits.T5IS = 0;      /* Timer 5 sub-priority: 0 */
    IEC0bits.T5IE = 1;      /* Set Timer 5 Interrupt Enable bit */
}
