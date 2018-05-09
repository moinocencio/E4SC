
#ifndef __PWM_H__
#define __PWM_H__

void pwm_v1(void); // First Version , f=19 kHz , dutycycle = 30 %
void set_pwm(uint32_t freq, uint32_t dcycle);
void set_pwm_dc(uint32_t dcycle); // Set Duty-Cucle
void testPWM_dc();
void pwm_config(void);
// OC2 - 5 , OC3 - 6, OC4 - 9
void pwm_config_lowfreq(void);

#endif