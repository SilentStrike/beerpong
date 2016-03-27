#include "softpwm.h"

uint8 pwm_duty_1;

void pwm_setup(void)
{
    pwm_duty_1 = 0;
    TRISBbits.TRISB7 = 0;
    
    
    LATBbits.LATB7 = 0;

}

void pwm_timer_callback(void)
{
    static uint8 counter;
    
    if(counter >= 100)
    {
        counter = 0;
        if(pwm_duty_1 != 0)
            LATBbits.LATB7 = 1;
        
    }
    else
    {        
        if (counter == pwm_duty_1)
        {
            LATBbits.LATB7 = 0;
        }
        
        
        ++counter;
    }
    
}