#include "common_inc.h"
#include "configurations.h"
#include "HelloWord/hw_keyboard.h"


/* Component Definitions -----------------------------------------------------*/
KeyboardConfig_t config;
HWKeyboard keyboard(&hspi1);

static bool isSoftWareControlColor = false;
static bool isReceiveSuccess = false;
/* Main Entry ----------------------------------------------------------------*/
void Main()
{
    EEPROM eeprom;
    eeprom.Pull(0, config);
    if (config.configStatus != CONFIG_OK)
    {
        // Use default settings
        config = KeyboardConfig_t{
            .configStatus = CONFIG_OK,
            .serialNum=123,
            .keyMap={}
        };
        memset(config.keyMap, -1, 128);
        eeprom.Push(0, config);
    }

    // Keyboard Report Start
    HAL_TIM_Base_Start_IT(&htim4);


    while (true)
    {
        /*---- This is a demo RGB effect ----*/
        static uint32_t t = 1;
        static bool fadeDir = true;

        fadeDir ? t++ : t--;
        if (t > 250) fadeDir = false;
        else if (t < 1) fadeDir = true;

        if(isReceiveSuccess){
            keyboard.SyncLights();
            isReceiveSuccess = false;
        }
        if(!isSoftWareControlColor)
        {

            /*---- This is a demo RGB effect ----*/
            for (uint8_t i = 0; i < HWKeyboard::LED_NUMBER; i++)
                keyboard.SetRgbBufferByID(i, HWKeyboard::Color_t{(uint8_t)t, 50, 20});
            /*-----------------------------------*/
            keyboard.SyncLights();
        }
    }
}

/* Event Callbacks -----------------------------------------------------------*/
extern "C" void OnTimerCallback() // 1000Hz callback
{
    keyboard.ScanKeyStates();  // Around 40us use 4MHz SPI clk
    keyboard.ApplyDebounceFilter(100);
    keyboard.Remap(keyboard.FnPressed() ? 2 : 1);  // When Fn pressed use layer-2

    if (keyboard.KeyPressed(HWKeyboard::LEFT_CTRL) &&
        keyboard.KeyPressed(HWKeyboard::A))
    {
        // do something...

        // or trigger some keys...
        keyboard.Press(HWKeyboard::DELETE);
    }

    // Report HID key states
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,
                               keyboard.GetHidReportBuffer(1),
                               HWKeyboard::KEY_REPORT_SIZE);
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    keyboard.isRgbTxBusy = false;
}

extern "C"
void HID_RxCpltCallback(uint8_t* _data)
{
    if(_data[1] == 0xbd)  isSoftWareControlColor= false;
    if(_data[1] == 0xac) {
        isSoftWareControlColor = true;
        uint8_t pageIndex = _data[2];
        for (uint8_t i = 0; i < 10; i++) {
            if(i+pageIndex*10>=HWKeyboard::LED_NUMBER) {
                isReceiveSuccess = true;
                break;
            }
            keyboard.SetRgbBufferByID(i+pageIndex*10,
                                      HWKeyboard::Color_t{_data[3+i*3], _data[4+i*3], _data[5+i*3]});
        }
    }
}