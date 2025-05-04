#include "w5500_hal.h"
#include "main.h"
#include "stm32f1xx_hal.h"   // HAL functions
#include "cmsis_os.h"        // If using FreeRTOS
#include "stm32f1xx_ll_usart.h"           // For debug (optional)

// SPI1 handle from CubeMX
extern SPI_HandleTypeDef hspi1;

// Adjust based on your CubeMX config
#define W5500_CS_GPIO   GPIOA
#define W5500_CS_PIN    GPIO_PIN_4

void wizchip_select(void)
{
    HAL_GPIO_WritePin(W5500_CS_GPIO, W5500_CS_PIN, GPIO_PIN_RESET);
}

void wizchip_deselect(void)
{
    HAL_GPIO_WritePin(W5500_CS_GPIO, W5500_CS_PIN, GPIO_PIN_SET);
}

void wizchip_write(uint8_t byte)
{
    HAL_SPI_Transmit(&hspi1, &byte, 1, HAL_MAX_DELAY);
}

uint8_t wizchip_read(void)
{
    uint8_t tx = 0x00, rx = 0x00;
    HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, HAL_MAX_DELAY);
    return rx;
}
