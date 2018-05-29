/* Closed Loop Shifter:
 *  1st iteration of rotation controlled by zero crossing
 * 
 * Program functioning:
 *      - Timer 5 imposes new phase, in case no zero crossing is detected
 *      - Timer 2 puts Outputs to HIGH, enables timer 3, and 
 * 
 * Since there's no feedback, the duty cycle of the function shall not be changed during
 * operation.
 */

#include "config_bits.h"
#include "timers.h"
#include <xc.h>            
#include <sys/attribs.h>    
#include <stdio.h>
#include <stdlib.h>

volatile int state = 0;         // Sequence Step  0 - 5
static uint32_t w8Counter = 50;
volatile uint32_t counter = 0;

void __ISR(8) isr_timer2(void)  // Actualizing drivers and setting PWM
{
    switch(state) 
    {
        case 0:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0018; // set
            break;
        case 1:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0009; // set
            break;
        case 2:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0021; // set
            break;
        case 3:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0024; // set
            break;
        case 4:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0006; // set
            break;
        case 5:
            LATE &= 0xFFC0; // clear
            LATE |= 0x0012; // set
            break;
        default:
            break;
    }
    TMR3 = 0;
    TMR2 = 0;
    IFS0bits.T3IF = 0;
    /* Set Timer 3 Interrupt Enable bit  */
    IEC0bits.T3IE = 1;     
    /* Reset flag */
    IFS0bits.T2IF = 0;
}

void __ISR(12) isr_timer3(void) // Set pins to LOW
{
    LATE &= 0xFFC0;
    TMR3 = 0;
    /* Disable Timer 3 Interrupt */
    IEC0bits.T3IE = 0;  
    /* Reset flag */
    IFS0bits.T3IF = 0;
    
}

void __ISR(20) isr_timer5(void) // Update State
{ 
    if(counter < w8Counter)
        counter++;
    else {
        if(PR5 > FINALPR5) PR5--;
        counter = 0;
    }
        
    if(state == 5)
        state = 0;
    else
        state++;

    IFS0bits.T5IF = 0; // Reset flag
}

void initial_configs()
{
    TRISG &= 0x8FFF;    // Define RG14-RA12 as outputs - 0
    LATG &= 0x8FFF;     // Turn off Shut down on drivers (RG14-RG12)
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    config_timer2();    // PWM HIGH configuration
    config_timer3();    // PWM LOW configuration
    config_timer5();    // Next phase Timer
    config_Inttimer2(); // PWM HIGH
    config_Inttimer3(); // PWM LOW
    IEC0bits.T2IE = 1;      // Set Timer 2 Interrupt Enable bit 
    config_Inttimer5(); // Control Timer
}
void main (void)
{
    initial_configs();
    
    enableInterrupts(); 
    
    while(1);
}