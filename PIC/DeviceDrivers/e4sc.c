#include "config_bits.h"
#include <xc.h>            
#include <sys/attribs.h> 

#include "uart.h"
#include "adc.h"
#include <stdio.h>
#include <stdlib.h>


// Interrupção da ADC
void __ISR(27) isr_adc(void)
{
    AD1CON1bits.ASAM = 1;
    int j =0;
    float res = 0, aux = 0, val = 0;
    // int res=0, aux=0, val=0;
	int *p = (int* ) (&ADC1BUF0);
	for(j = 0; j < 8; j++)
    {
		aux = p[j*4];
        val += (aux*3.3+65)/1023;
	}
    res = val/8;
    // printf("Vint: %d\n\r", res);
    IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

// External Interrupt 0 - Table 7.7
void __ISR(3) isr_extInt0(void)
{
    printf("\nExternal Interrupt_0\r");
    IFS0bits.INT0IF = 0; // Flag
}
void __ISR(7) isr_extInt1(void)
{
    printf("\nExternal Interrupt_1\r");
    IFS0bits.INT1IF = 0; // Flag
}
void __ISR(11) isr_extInt2(void)
{
    printf("\nExternal Interrupt_2\r");
    IFS0bits.INT2IF = 0; // Flag
}
void __ISR(15) isr_extInt3(void)
{
    printf("\nExternal Interrupt_3\r");
    IFS0bits.INT3IF = 0; // Flag
}
// Configuração de todas as interrupções
void config_interrupts()
{
    config_adcInt();
    config_extInt0(); // Detecção de Curto-Circuito
    config_extInt1();
    config_extInt2();
    config_extInt3();
}

void main (void)
{
    __builtin_disable_interrupts();
    config_uart();
    printf("\n\nUART Config\r");
    config_adcA0();
    printf("\nADC Config\r");
    enableInterrupts();
    config_interrupts();
    printf("\nInter Config\r");
    printf("\nINtsEnable\r");
    
    testPWM_dc();
    
    while(1)
    {
    }
}