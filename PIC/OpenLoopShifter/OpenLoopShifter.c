/* Open Loop Shifter:
 *  Set rotation of "motor" or other three-phase system through an open loop algorithm,
 *  in order to validate system functioning. Intended to be presented at 2nd Phase of 
 *  Eletrónica 4 course unit.
 * 
 * Program functioning:
 *      - Timer 5 imposes new phase, changing the terminals being excited
 *      - Timer 2 creates PWM signal on terminals imposed by variable state
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
volatile int laststate = 0;     // Last state of PWM  
volatile int counter = 0;       // 
volatile int duty_cycle = 1;    // 1 - 25% 2- 50% 3- 75% 4-100%

void __ISR(8) isr_timer2(void)  // Actualizing drivers and setting PWM
{
    if(counter < duty_cycle && laststate == 0) // && state != laststate)
    {
        switch(state) 
        {
            // Problema pin 15
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
                printf("\nDefault\r");
                break;
        }
        laststate = 1;
    }
    if(counter >= duty_cycle && laststate == 1)
    {
        LATE &= 0xFFC0; // clear
        laststate = 0;
    }    
    if(counter==duty_cycle+1)
    {
        counter = 0;
        laststate = 0;
    }
    else
    {
        counter++;
    }
    IFS0bits.T2IF = 0;     // Reset flag   
}

void __ISR(20) isr_timer5(void) // Update State
{ 
    if(state == 5)
        state = 0;
    else
        state++;

    IFS0bits.T5IF = 0; // Reset flag
}

void initial_configs()
{
    config_uart();      // UART Configuration
    TRISG &= 0x8FFF;    // Define RG14-RA12 as outputs - 0
    LATG &= 0x8FFF;     // Turn off Shut down on drivers (RG14-RG12)
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    config_timer2();    // PWM configuration
    config_timer5();    // Next phase Timer
    config_Inttimer2(); // PWM
    config_Inttimer5(); // Control Timer
}
void main (void)
{
    initial_configs();
    enableInterrupts(); 
    
    while(1)
    {
        
    }
}