#include "input_capture.h"
#include "sublibinal.h"

uint16 ic1_val;

void setup_input_capture(void)
{
    ic1_val = 5;
    
    //pin setup
    IC1R = 0b0010; //RPA4
    TRISAbits.TRISA4 = 1;
    
    //interrupt setup
    IFS0bits.IC1IF = 0; //clear the interrupt flag
    IPC1bits.IC1IP = 7; //set the priority of the interrupt to 7
    IEC0bits.IC1IE = 1; //enable the interrupt
    
    //timer3 setup
    T3CONbits.TCKPS = 0b111; //prescaler bits
    T3CONbits.ON = 1;    
    
    IC1CONbits.C32 = 0; //16-bit timer
    IC1CONbits.ICTMR = 0; //timer 3
    IC1CONbits.ICI = 0; //interrupt every capture
    IC1CONbits.ICM = 0b101; //every 16th rising edge
    IC1CONbits.ON = 1;
}


void __ISR(_INPUT_CAPTURE_1_VECTOR, IPL7AUTO) Input_Capture_1_Handler(void) {

    asm volatile ("di"); //disable interrupts
    
    ic1_val = IC1BUF;
    TMR3 = 0;

    IFS0bits.IC1IF = 0; //clear the interrupt flag

    asm volatile ("ei"); //reenable interrupts
}
