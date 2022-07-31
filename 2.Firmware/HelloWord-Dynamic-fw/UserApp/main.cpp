#include "common_inc.h"
#include "Platform/Sensor/Encoder/encoder.h"
#include "Platform/Driver/driver.h"
#include "Ctrl/Motor/motor.h"
#include "eink_290_bw.h"
#include "rgb_light.h"
#include "Ctrl/Motor/knob.h"
#include "U8g2lib.hpp"


/* User Variables ---------------------------------------------------------*/
BoardConfig_t boardConfig;
SSD1306 oled(&hi2c1);
Eink290BW eink;
Timer timerCtrlLoop(&htim7, 5000);
Encoder encoder(&hspi1);
Driver driver(12);
Motor motor = Motor(7);
RGB rgb(&hspi3);
KnobSimulator knob;


/* Thread Definitions -----------------------------------------------------*/
osThreadId_t ctrlLoopTaskHandle;
void ThreadCtrlLoop(void* argument)
{
    motor.AttachDriver(&driver);
    motor.AttachEncoder(&encoder);
    knob.Init(&motor);
    knob.SetEnable(true);
    knob.SetMode(KnobSimulator::MODE_DAMPED);

    for (;;)
    {
        // Suspended here until got Notification.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        knob.Tick();
    }
}

osThreadId_t oledTaskHandle;
void ThreadOledUpdate(void* argument)
{
    oled.Init();
    eink.Init();

    for (;;)
    {
        //OLED update
        oled.Clear();
        oled.SetDrawColor(1);
        oled.SetCursor(0, 0);
        oled.DrawUTF8(0, 10, "hello");
        oled.SendBuffer();


        //RGB update
        for (uint8_t i = 0; i < RGB::LED_NUMBER; i++)
            rgb.SetRgbBuffer(i, RGB::Color_t{128, 20, 0});

        rgb.SyncLights();
    }
}

osThreadId_t peripheralTaskHandle;
void ThreadGpioUpdate(void* argument)
{
    int mode = 0;

    for (;;)
    {
        if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port, KEY_A_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(KEY_A_GPIO_Port, KEY_A_Pin) == GPIO_PIN_RESET);
            mode++;
            if (mode > 5)mode = 0;
            knob.SetMode(static_cast<KnobSimulator::Mode_t>(mode));
        }
        if (HAL_GPIO_ReadPin(KEY_B_GPIO_Port, KEY_B_Pin) == GPIO_PIN_RESET)
        {
            while (HAL_GPIO_ReadPin(KEY_B_GPIO_Port, KEY_B_Pin) == GPIO_PIN_RESET);
            mode--;
            if (mode < 0)mode = 5;
            knob.SetMode(static_cast<KnobSimulator::Mode_t>(mode));
        }

        osDelay(20);
    }
}


/* Timer Callbacks -------------------------------------------------------*/
void OnTimerCallback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Wake & invoke thread IMMEDIATELY.
    vTaskNotifyGiveFromISR(TaskHandle_t(ctrlLoopTaskHandle), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


/* Default Entry -------------------------------------------------------*/
void Main(void)
{
    // Init all communication staff, include USB-CDC/VCP/UART/CAN etc.
    InitCommunication();

    // Init & Run User Threads.
    const osThreadAttr_t controlLoopTask_attributes = {
        .name = "ControlLoopTask",
        .stack_size = 4096,
        .priority = (osPriority_t) osPriorityRealtime, // robot control thread is critical, should be the highest
    };
    ctrlLoopTaskHandle = osThreadNew(ThreadCtrlLoop, nullptr, &controlLoopTask_attributes);

    const osThreadAttr_t oledTask_attributes = {
        .name = "OledTask",
        .stack_size = 4096,
        .priority = (osPriority_t) osPriorityNormal,   // should >= Normal
    };
    oledTaskHandle = osThreadNew(ThreadOledUpdate, nullptr, &oledTask_attributes);

    const osThreadAttr_t peripheralTask_attributes = {
        .name = "peripheralTask",
        .stack_size = 512,
        .priority = (osPriority_t) osPriorityNormal,
    };
    peripheralTaskHandle = osThreadNew(ThreadGpioUpdate, nullptr, &peripheralTask_attributes);


    // Start Timer Callbacks.
    timerCtrlLoop.SetCallback(OnTimerCallback);
    timerCtrlLoop.Start();
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    rgb.isRgbTxBusy = false;
}