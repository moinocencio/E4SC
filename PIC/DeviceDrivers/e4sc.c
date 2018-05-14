#include "config_bits.h"
#include <xc.h>            
#include <sys/attribs.h> 
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "timers.h"
#include "int.h"
#include <stdio.h>
#include <stdlib.h>

volatile int state = 0;         // Sequence Step  0 - 5
volatile int laststate = 0;     // Last state of pwm  
volatile int counter = 0;       // 
volatile int duty_cycle = 2;    // 1 - 25% 2- 50% 3- 75% 4-100%

// Delay random
void delay()
{
    int j = 0;
    for(j=0;j<900000;j++)  
    {
    }    
}
void send_seq(int state)
{
    switch(state) {
        case 0:
            LATF &= 0xCFF0; // clear
            LATF |= 0x1008; // set
            //printf("\n Seq 0 | A-%d, B-%d, C-%d  \r", Aseq[0],Bseq[0],Cseq[0]);
            break;
        case 1:
            LATF &= 0xCFF0; // clear
            LATF |= 0x0009; // set
            //printf("\n Seq 1 | A-%d, B-%d, C-%d  \r", Aseq[1],Bseq[1],Cseq[1]);
            break;
        case 2:
            LATF &= 0xCFF0; // clear
            LATF |= 0xC000; // set
            //printf("\n Seq 2 | A-%d, B-%d, C-%d  \r", Aseq[2],Bseq[2],Cseq[2]);
            break;
        case 3:
            LATF &= 0xCFF0; // clear
            LATF |= 0x2004; // set
            //printf("\n Seq 3 | A-%d, B-%d, C-%d  \r", Aseq[3],Bseq[3],Cseq[3]);
            break;
        case 4:
            LATF &= 0xCFF0; // clear
            LATF |= 0x0005; // set
            //printf("\n Seq 4 | A-%d, B-%d, C-%d  \r", Aseq[4],Bseq[4],Cseq[4]);
            break;
        case 5:
            LATF &= 0xCFF0; // clear
            LATF |= 0x1002; // set
            //printf("\n Seq 5 | A-%d, B-%d, C-%d  \r", Aseq[5],Bseq[5],Cseq[5]);
            break;
        default:
            printf("\nDefault\r");
            break;
    }        
}
// ADC Interrupt
void __ISR(27) isr_adc(void)
{
    int j =0;
    float val = 0, aux = 0;
	int *p = (int* ) (&ADC1BUF0);
	for(j = 0; j < 8; j++)
    {
		aux = p[j*4];
        val += (aux*3.3+12)/1023;   //  -> 12 - Calibration
	}
    val = val/8;
    printf("Vint: %f\n\r", val);
    delay();
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
    // AD1CON1bits.ASAM = 1; // Start Conversion
}
// OverCurInt - pin 2
void __ISR(7) isr_extInt1(void)
{
    printf("\nExternal Interrupt_1 - Over Current - pin2\r");
    // Shut Down Drivers
    LATG |= 0x7000;         // RG14-RG12 -> 1
    IFS0bits.INT1IF = 0;    // Reset Flag
}
// External Interrupt 2 - PhaseATrig - pin 7
void __ISR(11) isr_extInt2(void)
{
    printf("\nExternal Interrupt_2 - PhaseATrig - pin7 -- State: %d\r", state+1);
    INTCONbits.INT2EP = !INTCONbits.INT2EP; // Rising Edge <-> Falling Edge  
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    IFS0bits.INT2IF = 0; // Flag
}
// External Interrupt 3 - PhaseBTrig - pin 21
void __ISR(15) isr_extInt3(void)
{
    printf("\nExternal Interrupt_3 - PhaseBTrig - pin21 -- State: %d\r", state+1);
    INTCONbits.INT3EP = !INTCONbits.INT3EP; // Rising Edge <-> Falling Edge  
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    IFS0bits.INT3IF = 0; // Flag
}
// External Interrupt 4 - PhaseCTrig - pin 20
void __ISR(19) isr_extInt4(void)
{
    printf("\nExternal Interrupt_4 - PhaseCTrig -pin20 -- State: %d\r", state+1);
    INTCONbits.INT4EP = !INTCONbits.INT4EP; // Rising Edge <-> Falling Edge  
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    IFS0bits.INT4IF = 0; // Reset Flag
}
// Timer 2 Interrupt - Freq PWM
void __ISR(8) isr_timer2(void)
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
    if(counter==3)
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
// Control Timer 
void __ISR(20) isr_timer5(void)
{ 
    // Phase 3
    IFS0bits.T5IF = 0; // Reset flag
}
/*
void test_send_seq(int nr, int state,int Aseq[], int Bseq[], int Cseq[])
{
    int i = 0, k = 0;
    for(k=0;k<nr;k++)
    {
        for(i=0;i<6;i++)
        {
            send_seq(state,Aseq,Bseq,Cseq);
            delay();
            state++;
        }
        state = 0;
        printf("\n-------------------\r");
    }
}
 * */
void initial_configs()
{
    config_uart();      // UART Configuration
    TRISG &= 0x8FFF;    // Define RG14-RA12 as outputs - 0
    LATG &= 0x8FFF;     // Drivers On RG14-RG12 - 1
    TRISE &= 0xFFC0;    // Define RE0-RE5 as outputs
    config_timer2();    // PWM configuration
    // config_adcInt(); // ADC Interrupt
    config_extInt1();   // Over Current detect
    config_extInt2();   // PhaseATrig
    config_extInt3();   // PhaseBTrig
    config_extInt4();   // PhaseCTrig
    config_Inttimer2(); // PWM
    // config_Inttimer5(); // Control Timer
}
void main (void)
{
    initial_configs();
    enableInterrupts(); 
      
    while(1)
    {
        // testADC();   // Send BatVoltMeas and MeanCur to PC
    }
}