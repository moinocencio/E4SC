#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "esc.h"

volatile unsigned int driverMask;           // Mask of drivers to put to HIGH, on each state
volatile unsigned int state = 0;            // state of the system

void updateMask(void) {
    /* 
     * Update mask of drivers to put to HIGH, according to the actual value of state 
     * DOES NOT UPDATE STATE!     
     */
    switch(state) {
        case 0:         //B Low - C High 
            driverMask= 0x0018;
            break;
        case 1:         //B Low - A High
            driverMask= 0x0009; // set
            break;
        case 2:         //C Low - A High
            driverMask= 0x0021; // set
            break;
        case 3:         //C Low - B High
            driverMask = 0x0024; // set
            break;
        case 4:         //A Low - B High
            driverMask = 0x0006; // set
            break;
        case 5:         //A Low - C High
            driverMask= 0x0012; // set
            break;
        default:
            break;
    }
}

void updateInterrupts(void) {
    /*
     * Updates the interrupts on each state, disabling the one that gets to VCC/GROUND, and
     * enabling the floating phase.
     * DOES NOT UPDATE STATE!     
     */    
    switch(state) {
        case 0:         //B Low - C High 
            IEC0bits.INT2IE = 1;    // Enable A Trig
            IEC0bits.INT3IE = 0;    // Disable B Trig
            break;
        case 1:         //B Low - A High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT4IE = 1;    // Enable C Trig
            break;
        case 2:         //C Low - A High
            IEC0bits.INT3IE = 1;    // Enable B Trig
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 3:         //C Low - B High
            IEC0bits.INT3IE = 0;    // Disable B Trig
            IEC0bits.INT2IE = 1;    // Enable A Trig
            break;
        case 4:         //A Low - B High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT4IE = 1;    // Enable C Trig
            break;
        case 5:         //A Low - C High
            IEC0bits.INT4IE = 0;    // Disable C Trig
            IEC0bits.INT3IE = 1;    // Enable B Trig
            break;
        default:
            break;
    }
}

void updateAllInterrupts(void) {
    /*
     * Updates the interrupts on each state, disabling the one that gets to VCC/GROUND,
     * the one that was already on the previous state and enabling the floating phase.
     * DOES NOT UPDATE STATE!     
     */    
    switch(state) {
        case 0:         //B Low - C High 
            IEC0bits.INT2IE = 1;    // Enable A Trig
            IEC0bits.INT3IE = 0;    // Disable B Trig
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 1:         //B Low - A High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT3IE = 0;    // Disable B Trig
            IEC0bits.INT4IE = 1;    // Enable C Trig
            break;
        case 2:         //C Low - A High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT3IE = 1;    // Enable B Trig
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 3:         //C Low - B High
            IEC0bits.INT2IE = 1;    // Enable A Trig
            IEC0bits.INT3IE = 0;    // Disable B Trig
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 4:         //A Low - B High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT3IE = 0;    // Disable B Trig            
            IEC0bits.INT4IE = 1;    // Enable C Trig
            break;
        case 5:         //A Low - C High
            IEC0bits.INT2IE = 0;    // Disable A Trig
            IEC0bits.INT3IE = 1;    // Enable B Trig
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        default:
            break;
    }
}

void switchInterruptEdge(void) {
    /*
     * If the control timer ISR gets executed, it needs to switch the phase of the
     * previously floating phase.
     * DOES NOT UPDATE STATE!
     */
    
    switch(state) 
    {
        case 0:         //B Low - C High 
            INTCONbits.INT2EP = !INTCONbits.INT2EP; // Rising Edge <-> Falling Edge
            break;
        case 1:         //B Low - A High
            INTCONbits.INT4EP = !INTCONbits.INT4EP; // Rising Edge <-> Falling Edge  
            break;
        case 2:         //C Low - A High
            INTCONbits.INT3EP = !INTCONbits.INT3EP; // Rising Edge <-> Falling Edge  
            break;
        case 3:         //C Low - B High
            INTCONbits.INT2EP = !INTCONbits.INT2EP; // Rising Edge <-> Falling Edge
            break;
        case 4:         //A Low - B High
            INTCONbits.INT4EP = !INTCONbits.INT4EP; // Rising Edge <-> Falling Edge  
            break;
        case 5:         //A Low - C High
            INTCONbits.INT3EP = !INTCONbits.INT3EP; // Rising Edge <-> Falling Edge  
            break;
        default:
            break;
    }
}

void disableFloatingInterrupt(void) {
    /*
     * Disable Interrupt that was expected to be turned on the present phase and
     * reset flag 
     */
    switch(state) {
        case 0:         //B Low - C High 
            IFS0bits.INT2IF = 0; // Flag
            IEC0bits.INT2IE = 0;    // Disable A Trig            
            break;
        case 1:         //B Low - A High
            IFS0bits.INT4IF = 0; // Flag
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 2:         //C Low - A High
            IFS0bits.INT3IF = 0; // Flag
            IEC0bits.INT3IE = 0;    // Disable B Trig
            break;
        case 3:         //C Low - B High
            IFS0bits.INT2IF = 0; // Flag
            IEC0bits.INT2IE = 0;    // Disable A Trig
            break;
        case 4:         //A Low - B High
            IFS0bits.INT4IF = 0; // Flag
            IEC0bits.INT4IE = 0;    // Disable C Trig
            break;
        case 5:         //A Low - C High
            IFS0bits.INT3IF = 0; // Flag
            IEC0bits.INT3IE = 0;    // Disable B Trig
            break;
        default:
            break;
    }
}