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
#include <xc.h>            
#include <sys/attribs.h>    
#include <stdio.h>
#include <stdlib.h>

volatile uint8_t state = 0;

volatile uint8_t delta = 0;   
volatile uint8_t Lastdelta = 0;

volatile uint8_t emA;
volatile uint8_t emB;
volatile uint8_t emC;

volatile  uint8_t driverMask;           // Mask of drivers to put to HIGH, on each state

void __ISR(8,IPL1AUTO) isr_timer2(void)  // Set pins to HIGH and enabling T3
{
    LATE &= 0xFFC0;
    switch(state) 
    {
        // Problema pin 15
        case 0:            
            driverMask = 0x0018; // LATE |= 0x0018; // set
            //delta = emA;
            break;
        case 1:            
            driverMask = 0x0009; //LATE |= 0x0009; // set
            //delta = emC;
            break;
        case 2:            
            driverMask = 0x0021; //LATE |= 0x0021; // set
            //delta = emB;
            break;
        case 3:            
            driverMask = 0x0024; //LATE |= 0x0024; // set
            //delta = emA;
            break;
        case 4:
            driverMask = 0x0006; // LATE |= 0x0006; // set
            //delta = emC;
            break;
        case 5:
            driverMask = 0x0012; // LATE |= 0x0012; // set
            //delta = emB;
            break;
        default:
            break;
    }
    /*
    if (Lastdelta != delta) 
    {
        Lastdelta = delta; //save the last delta
        state = state + 1;
        if (state > 5) {
            state= 0;
        }
    }
    */
    LATE |= driverMask;
    TMR3 = 0;
    TMR2 = 0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;   /* Set Timer 3 Interrupt Enable bit  */      
    IFS0bits.T2IF = 0;  /* Reset flag */
}

void __ISR(12,IPL1AUTO) isr_timer3(void) // Set pins to LOW
{
    LATE &= 0xFFC0;
    TMR3 = 0;
    IEC0bits.T3IE = 0;   /* Disable Timer 3 Interrupt */ 
    IFS0bits.T3IF = 0;    /* Reset flag */
}
/*
void __ISR(20,IPL5AUTO) isr_timer5(void) // Update State
{ 
    emA = PORTEbits.RE9;
    emB = PORTAbits.RA14;
    emC = PORTAbits.RA15;
    switch(state) 
    {
        // Problema pin 15
        case 0:            
            //driverMask = 0x0018; // LATE |= 0x0018; // set
            delta = emA;
            break;
        case 1:            
            //driverMask = 0x0009; //LATE |= 0x0009; // set
            delta = emC;
            break;
        case 2:            
            //driverMask = 0x0021; //LATE |= 0x0021; // set
            delta = emB;
            break;
        case 3:            
            //driverMask = 0x0024; //LATE |= 0x0024; // set
            delta = emA;
            break;
        case 4:
            //driverMask = 0x0006; // LATE |= 0x0006; // set
            delta = emC;
            break;
        case 5:
            //driverMask = 0x0012; // LATE |= 0x0012; // set
            delta = emB;
            break;
        default:
            break;
    }
    if (Lastdelta != delta) 
    {
        Lastdelta = delta; //save the last delta
        state = state + 1;
        if (state > 5) {
            state= 0;
        }
    }
    TMR5 = 0;
    IFS0bits.T5IF = 0;    // Reset flag 
}
*/
void initial_configs()
{   
    // Shutdowns
    TRISG &= 0x8FFF;    // Define RG14-RG12 as outputs - 0
    LATG &= 0x8FFF;     // Turn off Shut down on drivers (RG14-RG12)
    // Drivers
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    LATE &= 0xFFC0;
    // External Interrupts
    //TRISEbits.TRISE8  = 1;
    TRISEbits.TRISE9  = 1;
    TRISAbits.TRISA14 = 1;
    TRISAbits.TRISA15 = 1;
    
    //config_uart();
    
    config_timer2();    // PWM HIGH configuration
    config_timer3();    // PWM LOW configuration
    //config_timer5();
    
    config_Inttimer2(); // PWM HIGH
    config_Inttimer3(); // PWM LOW
    //config_Inttimer5();
}

void main (void)
{
    initial_configs();
    // Reset Timers before initializing Interrupts
    TMR2 = 0;
    TMR3 = 0;  
    //TMR5 = 0; 
    enableInterrupts(); 
    
    while(1)
    {
        switch(state) 
        {
            case 0:            
                //driverMask = 0x0018; // LATE |= 0x0018; // set
                delta = PORTEbits.RE9;
                break;
            case 1:            
                //driverMask = 0x0009; //LATE |= 0x0009; // set
                delta = PORTAbits.RA15;
                break;
            case 2:            
                //driverMask = 0x0021; //LATE |= 0x0021; // set
                delta = PORTAbits.RA14;
                break;
            case 3:            
                //driverMask = 0x0024; //LATE |= 0x0024; // set
                delta = PORTEbits.RE9;
                break;
            case 4:
                //driverMask = 0x0006; // LATE |= 0x0006; // set
                delta = PORTAbits.RA15;
                break;
            case 5:
                //driverMask = 0x0012; // LATE |= 0x0012; // set
                delta = PORTAbits.RA14;
                break;
            default:
                break;
        }
        if (Lastdelta != delta) 
        {
            Lastdelta = delta; //save the last delta
            state++;
            if (state > 5) {
                state= 0;
            }
        }
    }
}