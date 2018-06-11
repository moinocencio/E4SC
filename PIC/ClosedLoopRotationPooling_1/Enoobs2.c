#include "config_bits.h"
#include "timers.h"
#include <xc.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

volatile uint8_t state = 0;

volatile uint8_t delta = 0;
volatile uint8_t Lastdelta = 0;

volatile uint8_t emA;
volatile uint8_t emB;
volatile uint8_t emC;

uint32_t i = 0;

volatile  uint8_t driverMask;           // Mask of drivers to put to HIGH, on each state

volatile uint8_t alignCount = 0;
static uint32_t maxAlignCount = 100;
volatile uint8_t maxSpeedFlag = 0;
volatile uint8_t calStage = 1;
volatile uint32_t timerVal = 9999;
volatile uint32_t pwmUpdateCount = 0;
static uint32_t maxPwmUpdateCount = 100;
volatile uint8_t mode = 0;
char bstr[8]; 

volatile uint32_t speed = 6099;

void __ISR(24,IPL3AUTO) isr_uart(void)
{   
    ReadString(bstr,8);   
    switch(bstr[4]){
        case 0x31:
            speed = 7999;
            break;
        case 0x32:
            speed = 7699;
            break;
        case 0x33:
            speed = 7499;
            break;
        case 0x34:
            speed = 7099;
            break;
        case 0x35:
            speed = 6899;
            break;
        case 0x36:
            speed = 6699;
            break;
        case 0x37:
            speed = 6499;
            break;
        case 0x38:
            speed = 6199;
            break;
        case 0x39:
            speed = 6099;
            break;
        case 0x63:
            mode = 1;
            break;
        case 0x76:
            mode = 0;
            break;
        default:
            mode = 0;
            break;
    }
    IFS0bits.U1RXIF = 0;
}

void __ISR(8,IPL1AUTO) isr_timer2(void)  // Set pins to HIGH and enabling T3
{
    if(alignCount < maxAlignCount) alignCount++;

    LATE &= 0xFFC0;
    switch(state)
    {
        case 0:
            driverMask = 0x0018; // LATE |= 0x0018; 
            break;
        case 1:
            driverMask = 0x0009; //LATE |= 0x0009; 
            break;
        case 2:
            driverMask = 0x0021; //LATE |= 0x0021; 
            break;
        case 3:
            driverMask = 0x0024; //LATE |= 0x0024; 
            break;
        case 4:
            driverMask = 0x0006; // LATE |= 0x0006;
            break;
        case 5:
            driverMask = 0x0012; // LATE |= 0x0012;
            break;
        default:
            break;
    }

    LATE |= driverMask;
    TMR3 = 0;
    TMR2 = 0;
    IFS0bits.T3IF = 0;          // Reset Timer 3 Interrupt flag
    IEC0bits.T3IE = 1;          // Set Timer 3 Enable bit
    IFS0bits.T2IF = 0;          // Reset Flag
}

void __ISR(12,IPL1AUTO) isr_timer3(void) // Set pins to LOW
{
    LATE &= 0xFFC0;
    TMR3 = 0;
    IEC0bits.T3IE = 0;          // Disable Timer 3 Interrupt
    IFS0bits.T3IF = 0;          // Reset Flag
}

void initial_configs()
{
    // Shutdowns
    TRISG &= 0x8FFF;            // Define RG14-RG12 as outputs - 0
    LATG &= 0x8FFF;             // Turn off Shut down on drivers (RG14-RG12)
    // Drivers
    TRISE &= 0xFFC0;            // Define RE0-RE5 as outputs
    LATE &= 0xFFC0;
    // External Interrupts
    //TRISEbits.TRISE8  = 1;
    TRISEbits.TRISE9  = 1;
    TRISAbits.TRISA14 = 1;
    TRISAbits.TRISA15 = 1;

    config_uart();

    config_timer2();            // PWM HIGH configuration
    config_timer3();            // PWM LOW configuration
    config_timer5();

    config_Inttimer2();         // PWM HIGH
    config_Inttimer3();         // PWM LOW
    config_U1_int();
}

void main (void)
{
    
    initial_configs();
    enableInterrupts();
    // Reset Timers before initializing Interrupts
    while(mode==0);
    IEC0bits.U1RXIE = 0;
    TMR2 = 0;
    TMR3 = 0;
    IEC0bits.T3IE = 1;      // Set Timer 2 Interrupt Enable bit 
    IEC0bits.T2IE = 1;      // Set Timer 2 Interrupt Enable bit 
    
    state = 3;
    
    PR3 = 699;
    
    state = 3;
    while(alignCount < maxAlignCount);
    state=4;
    alignCount = 0;
    while(alignCount < maxAlignCount);
    state=5;
    alignCount = 0;
    while(alignCount < maxAlignCount);
    state = 0;
    
    PR3 = 349;
    
    TMR5 = 0;
    
    while(1)
    {
        switch(state)
        {
            case 0:               
                delta = PORTEbits.RE9;
                break;
            case 1:
                delta = PORTAbits.RA15;
                break;
            case 2:
                delta = PORTAbits.RA14;
                break;
            case 3:
                delta = PORTEbits.RE9;
                break;
            case 4:
                delta = PORTAbits.RA15;
                break;
            case 5:
                delta = PORTAbits.RA14;
                break;
            default:
                break;
        }                

        if (Lastdelta != delta)
        {
            Lastdelta = delta; //save the last delta
            if (++state == 6) {
                state= 0;
            }
            if(!maxSpeedFlag) {
                if(PR2 > speed )
                    PR2--;
                else {
                    maxSpeedFlag = 1;
                    TMR5 = 0;
                }
            } else {
                if(calStage){
                    if(timerVal > TMR5 - 5 && timerVal < TMR5 + 5)
                        calStage = 0;
                    else
                        timerVal--;
                } else  {          
                    if(TMR5 > timerVal-100) {
                        if(++pwmUpdateCount == maxPwmUpdateCount) {
                            if(PR3 < 449)
                                PR3++;
                            pwmUpdateCount = 0;
                        }
                    } else if(TMR5 < timerVal) {
                        if(++pwmUpdateCount == maxPwmUpdateCount) {
                            if(PR3 > 309)
                                PR3 = PR3-5;
                            pwmUpdateCount = 0;
                        }
                    } else
                        PR3 = 349;
                }
                TMR5 = 0;
            }
        }
    }
}
