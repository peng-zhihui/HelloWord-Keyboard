#include <cstdio>
#include "eink_290_bw.h"
#include "pgmspace.h"
#include "stm32f4xx_hal.h"
#include "spi.h"


  uint8_t Eink290BW::buffer[EPD_HEIGHT * EPD_WIDTH / 8];

void Eink290BW::Init()
{
    HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
    HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, GPIO_PIN_SET);

    WaitUntilIdle();
    SendCommand(SW_RESET); // soft reset
    WaitUntilIdle();

    SendCommand(0x74); //set analog block control
    SendData(0x54);
    SendCommand(0x7E); //set digital block control
    SendData(0x3B);

    SendCommand(DRIVER_OUTPUT_CONTROL); //Driver output control
    SendData(0x27);
    SendData(0x01);
    SendData(0x00);

    SendCommand(DATA_ENTRY_MODE_SETTING); //data entry mode
    SendData(0x01);

    SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION); //set Ram-X address start/end position
    SendData(0x00);
    SendData(0x0F);    //0x0F-->(15+1)*8=128

    SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION); //set Ram-Y address start/end position
    SendData(0x27);   //0x0127-->(295+1)=296
    SendData(0x01);
    SendData(0x00);
    SendData(0x00);
    SendCommand(BORDER_WAVEFORM_CONTROL); //set border
    SendData(0x01);

    SendCommand(0x18); // use the internal temperature sensor
    SendData(0x80);


    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xB1);
    SendCommand(MASTER_ACTIVATION);

    SendCommand(SET_RAM_X_ADDRESS_COUNTER);
    SendData(0x00);
    SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    SendData(0x27);
    SendData(0x01);
    WaitUntilIdle();
    printf("e-Paper init OK!");
}

void Eink290BW::DrawBitmap(const unsigned char* datas)
{
    SendCommand(WRITE_RAM);   //write RAM for black(0)/white (1)

    for (int i = 0; i < SCREEN_BUFFER_SIZE; i++)
        SendData(PGM_READ_BYTES(&datas[i]));
}

void Eink290BW::Update()
{
    SendCommand(DISPLAY_UPDATE_CONTROL_1);
    SendData(0x40);

    SendCommand(DISPLAY_UPDATE_CONTROL_2);
    SendData(0xC7);
    SendCommand(MASTER_ACTIVATION);
    WaitUntilIdle();
}

void Eink290BW::DeepSleep()
{
    SendCommand(0x10); //enter deep sleep
    SendData(0x01);
}


void Eink290BW::SendCommand(unsigned char command)
{
    HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_RESET);
    SpiTransfer(command);
}

void Eink290BW::SendData(unsigned char data)
{
    HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, GPIO_PIN_SET);
    SpiTransfer(data);
}

void Eink290BW::WaitUntilIdle()
{
    while (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET)
        HAL_Delay(100);
}

void Eink290BW::SpiTransfer(unsigned char data)
{
    HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &data, 1, 5);
    HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, GPIO_PIN_SET);
}
