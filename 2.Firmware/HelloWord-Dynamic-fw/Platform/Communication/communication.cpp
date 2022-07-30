#include "communication.hpp"
#include "common_inc.h"


volatile bool endpointListValid = false;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

osThreadId_t commTaskHandle;
const osThreadAttr_t commTask_attributes = {
    .name = "commTask",
    .stack_size = 2048 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};


osThreadId_t usbIrqTaskHandle;
void UsbDeferredInterruptTask(void *ctx)
{
    (void) ctx; // unused parameter

    for (;;)
    {
        // Wait for signalling from USB interrupt (OTG_FS_IRQHandler)
        osStatus semaphore_status = osSemaphoreAcquire(sem_usb_irq, osWaitForever);
        if (semaphore_status == osOK)
        {
            // We have a new incoming USB transmission: handle it
            HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
            // Let the irq (OTG_FS_IRQHandler) fire again.
            HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
        }
    }
}


void InitCommunication(void)
{
    printf("\r\nHello, Ctrl-FOC v%.1f Started!\r\n", CONFIG_FW_VERSION);

    // Start command handling thread
    commTaskHandle = osThreadNew(CommunicationTask, nullptr, &commTask_attributes);

    while (!endpointListValid)
        osDelay(1);
}


// Thread to handle deffered processing of USB interrupt, and
// read commands out of the UART DMA circular buffer
void CommunicationTask(void *ctx)
{
    (void) ctx; // unused parameter

    CommitProtocol();

    // Allow main init to continue
    endpointListValid = true;

    StartUartServer();
    StartUsbServer();

    for (;;)
    {
        osDelay(1000); // nothing to do
    }
}

extern "C" {
int _write(int file, const char *data, int len);
}

// @brief This is what printf calls internally
int _write(int file, const char *data, int len)
{
    usb_stream_output_ptr->process_bytes((uint8_t *) data, len, nullptr);
    uart2_stream_output_ptr->process_bytes((uint8_t *) data, len, nullptr);
    return len;
}
