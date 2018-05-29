/* Closed Loop Shifter:
 *  1st iteration of rotation controlled by zero crossing
 * 
 * Situations where state gets updated:
 *  - Control Timer ISR
 *  - External Interrupts
 * 
 * When state gets updated:
 *  - Update Mask
 *  - Change floating phase Interrupt trigger
 *  - Change Interrupts
 * 
 * The motor starts rotating in open loop, and after a number of correct successive zero
 * crossings by phase A, the system begins to rotate in closed loop, returning to open 
 * loop after a defined number of consecutive fails.
 */

#include "config_bits.h"
#include "timers.h"
#include "esc.h"
#include <xc.h>            
#include <sys/attribs.h>    
#include <stdio.h>
#include <stdlib.h>

volatile uint32_t zcCounter = 0;
volatile uint8_t feedbackFlag = 0;  // 0 - Open Loop/ 1 - Closed Loop
volatile uint32_t failCounter = 0;
static uint32_t zcTresh = 12;

void __ISR(8,IPL3AUTO) isr_timer2(void)  // Set pins to HIGH and enabling T3
{
    LATE |= driverMask;
    TMR3 = 0;
    TMR2 = 0;
    IFS0bits.T3IF = 0;
    /* Set Timer 3 Interrupt Enable bit  */
    IEC0bits.T3IE = 1;     
    /* Reset flag */
    IFS0bits.T2IF = 0;
}

void __ISR(12,IPL3AUTO) isr_timer3(void) // Set pins to LOW
{
    LATE &= NdriverMask;
    TMR3 = 0;
    /* Disable Timer 3 Interrupt */
    IEC0bits.T3IE = 0;  
    /* Reset flag */
    IFS0bits.T3IF = 0;    
}

void __ISR(20,IPL4AUTO) isr_timer5(void) // Control Timer
{ 
    /*
     * If this ISR gets executed, it means that the motor is rotating below the expected
     * velocity. Therefore, it switches the interrupt flag of the floating phase, updates the interrupts,
     * forces the next state and updates the masks
     */
    
    if(zcCounter == zcTresh )
        feedbackFlag = 1;
        
    else if(feedbackFlag == 1)
        failCounter++;
    
    if(failCounter == zcTresh) {
        feedbackFlag = 0;
        zcCounter = 0;
    }
    
    updateAllInterrupts();
    switchInterruptEdge();
        
    if(state == 5)
        state = 0;
    else
        state++;
    
    updateMask();
    
    IFS0bits.T5IF = 0; // Reset flag
}

void __ISR(7,IPL6AUTO) isr_extInt1(void) // Over Current Interrupt
{
    // Shut Down Drivers
    LATG |= 0x7000;         // RG14-RG12 -> 1
    IFS0bits.INT1IF = 0;    // Reset Flag
}

void __ISR(11,IPL5AUTO) isr_extInt2(void) // External Interrupt 2 - PhaseATrig - pin 7
{
    LATE &= NdriverMask;
    
    if(!feedbackFlag)
        zcCounter++;
    else{     
        TMR5 = 0;
        if(state == 5)
            state = 0;
        else
            state++;

        updateMask();
        updateInterrupts();
    }
    
    IFS0bits.INT2IF = 0; // Flag
    INTCONbits.INT2EP = !INTCONbits.INT2EP; // Rising Edge <-> Falling Edge
}

void __ISR(15,IPL5AUTO) isr_extInt3(void) // External Interrupt 3 - PhaseBTrig - pin 21
{
    if(!feedbackFlag)
        zcCounter++;    
    else { 
        LATE &= NdriverMask;

        TMR5 = 0;
        if(state == 5)
            state = 0;
        else
            state++;

        updateMask();
        updateInterrupts();
    }
    
    IFS0bits.INT3IF = 0; // Flag
    INTCONbits.INT3EP = !INTCONbits.INT3EP; // Rising Edge <-> Falling Edge
}

void __ISR(19,IPL5AUTO) isr_extInt4(void) // External Interrupt 4 - PhaseCTrig - pin 20
{
    if(!feedbackFlag) 
        zcCounter++;   
    else{
        LATE &= NdriverMask;

        TMR5 = 0;
        if(state == 5)
            state = 0;
        else
            state++;

        updateMask();
        updateInterrupts();
    }
    
    IFS0bits.INT4IF = 0; // Flag
    INTCONbits.INT4EP = !INTCONbits.INT4EP; // Rising Edge <-> Falling Edge
}

void initial_configs()
{   
    // Shutdowns
    TRISG &= 0x8FFF;    // Define RG14-RG12 as outputs - 0
    LATG &= 0x8FFF;     // Turn off Shut down on drivers (RG14-RG12)
    // Drivers
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    LATE &= 0xFFC0;
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
    //config_extInt1();
    config_extInt2();
    config_extInt3();
    config_extInt4();
    
    config_Inttimer5(); // Control Timer
}

void main (void)
{
    initial_configs();
    // Reset Timers before initializing Interrupts
    TMR2 = 0;
    TMR3 = 0;
    TMR5 = 0;
    
    enableInterrupts(); 
    
    while(1);
}