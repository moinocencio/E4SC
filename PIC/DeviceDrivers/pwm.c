#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "pwm.h"

// Primeira versão do PWM
void pwm_v1(void)
{
    T3CONbits.TCKPS = 2;
	PR3 = 512; 
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
	// timer configuration sequence)
	OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC2CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC2RS = 128; // Ton constant
	OC2CONbits.ON = 1; // Enable OC2 module
}
// 3 Outputs
void pwm_config(void)
{
    T3CONbits.TCKPS = 2;
	PR3 = 512; 
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
	// timer configuration sequence)
	OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC2CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC2RS = 128; // Ton constant
	OC2CONbits.ON = 1; // Enable OC2 module
    
    OC3CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC3CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC3RS = 128; // Ton constant
    OC3CONbits.ON = 1; // Enable OC2 module
    
    OC4CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC4CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC4RS = 128; // Ton constant
    OC4CONbits.ON = 1; // Enable OC2 module
}
// 3 Outputs
/*
void pwm_config_lowfreq(void)
{
    T2CONbits.TCKPS = 2;
    T2CONbits.T32 = 1;
	PR2 = 312499; 
	TMR2 = 0; 
	T2CONbits.TON = 1; 
	
    
    // timer configuration sequence)
	OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC2CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC2RS = 156250; // Ton constant
	OC2CONbits.ON = 1; // Enable OC2 module
    
    OC3CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC3CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC3RS = 156250; // Ton constant
    OC3CONbits.ON = 1; // Enable OC2 module
    
    OC4CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC4CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC4RS = 156250; // Ton constant
    OC4CONbits.ON = 1; // Enable OC2 module
}
 */
void set_pwm(uint32_t freq, uint32_t dcycle)
{
    T3CONbits.TCKPS = 2; // 1:32 - 625 Hz % 16 ou 32 bits
    // PR3 = 512; 
    TMR3 = 0;
    T3CONbits.TON = 1;
    OC2CONbits.OCM = 6;
    OC2CONbits.OCTSEL = 1;
    PR3 = (40000000/(2^(T3CONbits.TCKPS))/freq )-1;
    OC2RS = ((PR3 + 1) * dcycle)/100;
    OC2CONbits.ON = 1; // Enable OC1 module
}
// Mantém a frequencia, varia o duty-cycle
void set_pwm_dc(uint32_t dcycle)
{
    T3CONbits.TCKPS = 2;
	PR3 = 512; 
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
	// timer configuration sequence)
	OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC2CONbits.OCTSEL =1;// Use timer T3 as the time base for PWM generation
	OC2RS = ((PR3 + 1) * dcycle)/100;
	OC2CONbits.ON = 1; // Enable OC1 module
}

// Varia o duty_cycle
void testPWM_dc()
{
    uint32_t i=0, j=0;
    int dc[] = {20,40,70,100};
    for(j=0;j<4;j++)
    {
        set_pwm_dc(dc[j]); 
        printf("\ndc = %i\r",dc[j]);
        for(i=0; i<1000000;i++)
        {    
        }
    }
}