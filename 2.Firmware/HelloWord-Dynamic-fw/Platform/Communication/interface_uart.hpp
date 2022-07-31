#ifndef __INTERFACE_UART_HPP
#define __INTERFACE_UART_HPP

#ifdef __cplusplus

#include "BSP/fibre/inc/protocol.hpp"

extern StreamSink *uart2_stream_output_ptr;

extern "C" {
#endif

#include <cmsis_os.h>

extern osThreadId uart_thread;

void StartUartServer(void);

#ifdef __cplusplus
}
#endif

#endif // __INTERFACE_UART_HPP
