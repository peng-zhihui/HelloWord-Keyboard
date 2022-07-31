#include <cmsis_os.h>
#include "stm32f4xx_hal.h"
#include "main.h"
#include "u8g2.h"


#define DEVICE_ADDRESS    0x3C
#define TX_TIMEOUT        1000

#define USE_I2C_INTERFACE
#define USE_SPI_INTERFACE


/* SPI Interface */
SPI_HandleTypeDef* U8G2_SPI_HANDLE;

/* I2C Interface */
I2C_HandleTypeDef* U8G2_I2C_HANDLE;


uint8_t u8x8_stm32_gpio_and_delay(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
    /* STM32 supports HW SPI, Remove unused cases like U8X8_MSG_DELAY_XXX & U8X8_MSG_GPIO_XXX */
    switch (msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            /* Insert codes for initialization */
            break;
        case U8X8_MSG_DELAY_MILLI:
            /* ms Delay */
            osDelay(arg_int);
            break;

#ifdef USE_SPI_INTERFACE
            /* SPI Interface */
        case U8X8_MSG_GPIO_CS:
            /* Insert codes for SS pin control */
            break;
        case U8X8_MSG_GPIO_DC:
            /* Insert codes for DC pin control */
            break;
        case U8X8_MSG_GPIO_RESET:
            /* Insert codes for RST pin control */
            break;
#endif
    }
    return 1;
}


#ifdef USE_I2C_INTERFACE

uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[32];
    static uint8_t buf_idx;
    uint8_t* data;

    switch (msg)
    {
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t*) arg_ptr;
            while (arg_int > 0)
            {
                buffer[buf_idx++] = *data;
                data++;
                arg_int--;
            }
            break;
        case U8X8_MSG_BYTE_INIT:
            /* add your custom code to init i2c subsystem */
            break;
        case U8X8_MSG_BYTE_SET_DC:
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            buf_idx = 0;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            HAL_I2C_Master_Transmit(U8G2_I2C_HANDLE, (DEVICE_ADDRESS << 1), buffer, buf_idx, TX_TIMEOUT);
            break;
        default:
            return 0;
    }
    return 1;
}

#endif
