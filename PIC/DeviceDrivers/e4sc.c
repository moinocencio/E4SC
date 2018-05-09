#include "config_bits.h"
#include <xc.h>            
#include <sys/attribs.h> 

#include "uart.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>

volatile int state = 0;   // Indica fase da sequencia  0 - 5

void send_seq(int state)//, int Aseq[], int Bseq[], int Cseq[]) 
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
// Interrupção da ADC
void __ISR(27) isr_adc(void)
{
    int count = 500000, i = 0;
    AD1CON1bits.ASAM = 1; // Start Conversion
    int j =0;
    float res = 0, aux = 0, val = 0;
	int *p = (int* ) (&ADC1BUF0);
	for(j = 0; j < 8; j++)
    {
		aux = p[j*4];
        val += (aux*3.3+65)/1023; // 65 - constante de calibração
	}
    res = val/8;
    printf("Vint: %f\n\r", res);
    for(i=0;i<count;i++)
    {
    }
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}
// OverCurInt - pin 2
void __ISR(7) isr_extInt1(void)
{
    printf("\nExternal Interrupt_1 - Over Current - pin2\r");
    IFS0bits.INT1IF = 0; // Flag
}
// External Interrupt 2 - PhaseATrig - pin 7
void __ISR(11) isr_extInt2(void)
{
    printf("\nExternal Interrupt_2 - PhaseATrig - pin7\r");
    INTCONbits.INT2EP = !INTCONbits.INT2EP; 
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    // send_seq(state); 
    IFS0bits.INT2IF = 0; // Flag
}
// External Interrupt 3 - PhaseBTrig - pin 21
void __ISR(15) isr_extInt3(void)
{
    printf("\nExternal Interrupt_3 - PhaseBTrig - pin21\r");
    INTCONbits.INT3EP = !INTCONbits.INT3EP;
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    // send_seq(state);
    IFS0bits.INT3IF = 0; // Flag
}
// External Interrupt 4 - PhaseCTrig - pin 20
void __ISR(19) isr_extInt4(void)
{
    printf("\nExternal Interrupt_4 - PhaseCTrig -pin20\r");
    INTCONbits.INT4EP = !INTCONbits.INT4EP;
    if(state == 5)
    {
        state = 0;
    }
    else
    {
        state++;
    }
    // send_seq(state);
    IFS0bits.INT4IF = 0; // Flag
}
// Timer 2 Interrupt - Freq PWM
void __ISR(8) isr_timer2(void)
{
    // Colocar pins a High
    // send_seq
    printf("\nPWM start\r");
    TMR5 = 0;
    IEC0bits.T5IE = 1; // duty-cycle
    switch(state) {
        case 0:
            LATF &= 0xCFF0; // clear
            LATF |= 0x1008; // set
            break;
        case 1:
            LATF &= 0xCFF0; // clear
            LATF |= 0x0009; // set
            break;
        case 2:
            LATF &= 0xCFF0; // clear
            LATF |= 0xC000; // set
            break;
        case 3:
            LATF &= 0xCFF0; // clear
            LATF |= 0x2004; // set
            break;
        case 4:
            LATF &= 0xCFF0; // clear
            LATF |= 0x0005; // set
            break;
        case 5:
            LATF &= 0xCFF0; // clear
            LATF |= 0x1002; // set
            break;
        default:
            printf("\nDefault\r");
            break;
    }
    // IEC0bits.T2IE = 0;
    TMR2 = 0;
}
// PWM - Duty_Cycle
void __ISR(20) isr_timer5(void)
{
    // Colocar pins a Low
    printf("\nDuty_cycle\r");
    switch(state) {
        case 0:
            LATF &= 0xCFF0; // clear
            break;
        case 1:
            LATF &= 0xCFF0; // clear
            break;
        case 2:
            LATF &= 0xCFF0; // clear
            break;
        case 3:
            LATF &= 0xCFF0; // clear
            break;
        case 4:
            LATF &= 0xCFF0; // clear
            break;
        case 5:
            LATF &= 0xCFF0; // clear
            break;
        default:
            printf("\nDefault\r");
            break;
    }
    TMR5 = 0;
    IEC0bits.T5IE = 0;
}
// Configuração de todas as interrupções
void config_interrupts()
{
    // config_adcInt();
    config_extInt1(); // Detecção de Curto-Circuito
    config_extInt2();
    config_extInt3();
    config_extInt4();
    config_Inttimer2();
    config_Inttimer5();
    printf("\nInts Config\r");
}
// Delay random
void delay()
{
    int j = 0;
    for(j=0;j<500000;j++)
    {
    }    
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
    TRISF &= 0xCFF0;
}
void main (void)
{
    config_uart();
    initial_configs();
    // config_adcA0();
    // config_adcInt();
    config_interrupts();
    enableInterrupts();
        
    while(1)
    {
    }

}