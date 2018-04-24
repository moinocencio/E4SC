#include "config_bits.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

void main (void)
{
    config_uart();
    float res=0;
    config_adc();
    set_pwm(); // set_pwm
    
    while(1) 
    {
       // Get one sample
       IFS1bits.AD1IF = 0; // Reset interrupt flag
       AD1CON1bits.ASAM=1; // Start conversion
       while(IFS1bits.AD1IF == 0); // Wait for EOC
       // Convert to 0..3.3V
       res = (ADC1BUF0 * 3.3) / 1023;
       // Output result
       printf("Voltage: %f\n\r",res);
    }    
}

