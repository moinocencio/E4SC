/* Closed Loop Shifter:
 *  1st iteration of rotation controlled by zero crossing
 * 
 * Program functioning:
 *      - Timer 5 imposes new phase, in case no zero crossing is detected
 *      - Timer 2 puts Outputs to HIGH, enables timer 3, and 
 * 
 * State update:
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
volatile uint32_t closedLoopCounter = 0;
static uint32_t maxLoopCounter = 40;

void __ISR(8) isr_timer2(void)  // Actualizing drivers and setting PWM
{
    switch(state) 
    {
        case 0:         //B Low - C High 
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT2IE = 1;    // Enable A Trig
                IEC0bits.INT3IE = 0;    // Disable B Trig
            }
            LATE &= 0xFFC0; // clear
            LATE |= 0x0018; // set
            break;
        case 1:         //B Low - A High
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT2IE = 0;    // Disable A Trig
                IEC0bits.INT4IE = 1;    // Enable C Trig
            }
            LATE &= 0xFFC0; // clear
            LATE |= 0x0009; // set
            break;
        case 2:         //C Low - A High
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT3IE = 1;    // Enable B Trig
                IEC0bits.INT4IE = 0;    // Disable C Trig
            }
            LATE &= 0xFFC0; // clear
            LATE |= 0x0021; // set
            break;
        case 3:         //C Low - B High
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT3IE = 0;    // Disable B Trig
                IEC0bits.INT2IE = 1;    // Enable A Trig
            }
            LATE &= 0xFFC0; // clear
            LATE |= 0x0024; // set
            break;
        case 4:         //A Low - B High
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT2IE = 0;    // Disable A Trig
                IEC0bits.INT4IE = 1;    // Enable C Trig
            }
            LATE &= 0xFFC0; // clear
            LATE |= 0x0006; // set
            break;
        case 5:         //A Low - C High
            if(closedLoopCounter >= maxLoopCounter) {
                IEC0bits.INT4IE = 0;    // Disable C Trig
                IEC0bits.INT3IE = 1;    // Enable B Trig
            }
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

void __ISR(20) isr_timer5(void) // Control Timer
{ 
    if(state == 5)
        state = 0;
    else
        state++;

    IFS0bits.T5IF = 0; // Reset flag
}

// Over Current Interrupt
void __ISR(7) isr_extInt1(void)
{
    // Shut Down Drivers
    LATG |= 0x7000;         // RG14-RG12 -> 1
    IFS0bits.INT1IF = 0;    // Reset Flag
}

// External Interrupt 2 - PhaseATrig - pin 7
void __ISR(11) isr_extInt2(void)
{
    if(closedLoopCounter < maxLoopCounter && state == 0)
        closedLoopCounter++;
    else {
        IEC0bits.T5IE = 0;
        INTCONbits.INT2EP = !INTCONbits.INT2EP; // Rising Edge <-> Falling Edge  
        if(state == 5)
            state = 0;
        else
            state++;
    }
    IFS0bits.INT2IF = 0; // Flag
}

// External Interrupt 3 - PhaseBTrig - pin 21
void __ISR(15) isr_extInt3(void)
{
    INTCONbits.INT3EP = !INTCONbits.INT3EP; // Rising Edge <-> Falling Edge  
    if(state == 5)
        state = 0;
    else
        state++;
    IFS0bits.INT3IF = 0; // Flag
}

// External Interrupt 4 - PhaseCTrig - pin 20
void __ISR(19) isr_extInt4(void)
{
    INTCONbits.INT4EP = !INTCONbits.INT4EP; // Rising Edge <-> Falling Edge  
    if(state == 5)
        state = 0;
    else
        state++;
    IFS0bits.INT4IF = 0; // Reset Flag
}

void initial_configs()
{   
    // Shutdowns
    TRISG &= 0x8FFF;    // Define RG14-RG12 as outputs - 0
    LATG &= 0x8FFF;     // Turn off Shut down on drivers (RG14-RG12)
    // Drivers
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    // External Interrupts
    TRISEbits.TRISE8 = 1;
    TRISEbits.TRISE9 = 1;
    TRISAbits.TRISA14 = 1;
    TRISAbits.TRISA15 = 1;
    
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