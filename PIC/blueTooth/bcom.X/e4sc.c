#include "config_bits.h"
#include <stdint.h>
#include <xc.h>            
#include <sys/attribs.h> 
#include "uart.h"
#include "int.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char bstr[8]; 
volatile uint32_t mode = 0;

// Uart Interrupt
void __ISR(24,IPL3AUTO) isr_uart(void)
{   
    ReadString(bstr,8);   
    if (strcmp(bstr, "mode1111") == 0) 
    {
        mode = 1;
    } 
    else if (strcmp(bstr, "mode2222") == 0)
    {
        mode = 2;
    }
    else 
    {
        mode = 0;
    }
    IFS0bits.U1RXIF = 0;
}
void main (void)
{
    config_uart();
    config_U1_int();
    enableInterrupts();
    while(1)
    {                      
        // Enviar 8bits + \n      
        // printf("\nResponse: %s\n\r", bstr);       
    }
}

