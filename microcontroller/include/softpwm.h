/* 
 * File:   pwm.h
 * Author: james
 *
 * Created on March 26, 2016, 12:30 PM
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "System.h"
    
extern uint8 pwm_duty_1;

void pwm_setup(void);
void pwm_timer_callback(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

