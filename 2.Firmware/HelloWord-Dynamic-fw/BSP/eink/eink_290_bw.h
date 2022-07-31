#ifndef __DEPG0290BxS75AFxX_BW_H__
#define __DEPG0290BxS75AFxX_BW_H__

/* EPD commands */
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF
/* Display resolution */
#define EPD_WIDTH          128
#define EPD_HEIGHT         296
#define SCREEN_BUFFER_SIZE   (EPD_WIDTH * EPD_HEIGHT / 8)


class Eink290BW
{
public:
    Eink290BW()
    = default;

    void Init();
    void Update();
    void DeepSleep();
    void DrawBitmap(const unsigned char* datas);

    static uint8_t buffer[EPD_HEIGHT * EPD_WIDTH / 8];


private:
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle();
    void SpiTransfer(unsigned char data);
};

#endif
