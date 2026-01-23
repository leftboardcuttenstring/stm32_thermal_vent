#include "stm32f4xx_hal.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;
I2C_HandleTypeDef i2c;

uint8_t* cmd;
uint8_t cmd_tmp[3] = {0};

bool set_temperature_cmd_flag = false;
bool turn_off_cmd_flag = false;
bool info_cmd_flag = false;
bool data_incoming = false;

extern uint8_t rx_byte;
extern uint8_t rx_buffer[64];
extern uint8_t rx_buffer_index;
extern uint8_t command_ready;

/*--Function headers for STM32-------------------------------------------------*/

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void i2c_init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  i2c_init();
  
  HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
  
  while (1)
  {
    if (command_ready == 1) {
      HAL_UART_Transmit(&huart2, rx_buffer, strlen((char*)rx_buffer), 100);
      HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 10);

      rx_buffer_index = 0;
      memset(rx_buffer, 0, 64);
      command_ready = 0;
    }
  }
}

void find_cmd(uint8_t* msg) {
  uint8_t cmd[3] = {0};
  for(int i = 0; i < 3; i++) {
    cmd_tmp[i] = msg[i];
  }
}

void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void i2c_init(void)
{
  i2c.Instance = I2C1;
  i2c.Init.ClockSpeed = 100000;
  i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  i2c.Init.OwnAddress1 = 0;
  i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  i2c.Init.OwnAddress2 = 0;
  i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&i2c) == HAL_OK) {
    HAL_UART_Transmit(&huart2, (const uint8_t *)"fine\n", strlen((char *)"fine\n"), HAL_MAX_DELAY);
  }
}

void MX_GPIO_Init(void)
{
    //PC7 - base/gate
    //PB6 - collector
    //PA7 - emitter
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_7;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio);
    HAL_GPIO_Init(GPIOA, &gpio);
    /* --- */
    gpio.Pin = GPIO_PIN_6;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
}

void _init(void) {

}
