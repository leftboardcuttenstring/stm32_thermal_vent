#ifndef STM32F401xE
#define STM32F401xE
#endif

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define set_temperature_cmd "set\0"
#define turn_off_cmd "sdn\0"

void MX_USART2_UART_Init(void);
void i2c_init(void);
void MX_GPIO_Init(void);
void Error_Handler(char* msg);
void find_cmd(uint8_t* msg);

int set_pwm(uint8_t temp);

extern bool set_temperature_cmd_flag;
extern bool turn_off_cmd_flag;
extern bool info_cmd_flag;
extern bool data_incoming;

#ifdef __cplusplus
}
#endif

#endif
