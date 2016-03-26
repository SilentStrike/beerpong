/* 
 * File:   main.c
 * Author: james
 *
 * Created on March 26, 2016, 11:22 AM
 */

#include "sublibinal.h"
#include "chip_config.h"
#include "softpwm.h"

void status_timer_callback(void);
void packetizer_callback(uint8* data, uint8 size);

typedef enum {
    MOTOR_PWM
    
}PACKTIZER_CODES;


int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];
    
    //config structures
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    Packetizer_Config packetizer_config = {0};
    
    //timer setup
    timer_config.frequency = 1;
    timer_config.which_timer = Timer_1;
    timer_config.pbclk = PB_CLK;
    timer_config.enabled = 1;
    timer_config.callback = &status_timer_callback;
    initialize_Timer(timer_config);
    
    timer_config.frequency = 10000;
    timer_config.which_timer = Timer_2;
    timer_config.pbclk = PB_CLK;
    timer_config.enabled = 1;
    timer_config.callback = &pwm_timer_callback;
    initialize_Timer(timer_config);
    
    
    
    //computer comm setup
    uart_config.which_uart = UART_CH_1;
    uart_config.pb_clk = PB_CLK;
    uart_config.speed = 115200;
    uart_config.rx_buffer_ptr = uart_rx_buffer;
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer);
    uart_config.tx_buffer_ptr = uart_tx_buffer;
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer);
    uart_config.rx_pin = Pin_RPB13;
    uart_config.tx_pin = Pin_RPB15;
    uart_config.tx_en = 1;
    uart_config.rx_en = 1;
    
    packetizer_config.callback = &packetizer_callback;
    packetizer_config.control_byte = 0x0A;
    packetizer_config.uart_config = uart_config;
    packetizer_config.which_channel = PACKET_UART_CH_1;
    initialize_packetizer(packetizer_config);
    
    
    pwm_setup();
    
    //Global interrupt enable. Do this last!
    enable_Interrupts();

    while (1) {
        bg_process_packetizer(PACKET_UART_CH_1);
    }

    return 0;
}

void status_timer_callback(void)
{
    uint8 data[3];

    data[0] = 'a';
    data[1] = 'b';
    data[2] = 'c';

    send_packet(PACKET_UART_CH_1, data, sizeof(data));
}

void packetizer_callback(uint8* data, uint8 size)
{
    switch(data[0])
    {
        case MOTOR_PWM:
            pwm_duty_1 = data[1];
            break;
            
    }
}