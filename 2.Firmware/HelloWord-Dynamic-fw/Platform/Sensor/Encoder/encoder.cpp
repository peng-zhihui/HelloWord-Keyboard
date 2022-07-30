#include "encoder.h"
#include "spi.h"


void Encoder::SpiInit()
{
    MX_SPI1_Init();
}


uint16_t Encoder::SpiTransmitAndRead16Bits(uint16_t _dataTx)
{
    uint16_t dataRx;

    M0_CS_GPIO_Port->BSRR = (uint32_t) M0_CS_Pin << 16U; // Chip select

#if 0
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &_dataTx, (uint8_t*) &dataRx, 1, HAL_MAX_DELAY);
#else
    /* Set the transaction information */
    spiHandle->pRxBuffPtr = (uint8_t*) (&dataRx);
    spiHandle->RxXferCount = 1;
    spiHandle->pTxBuffPtr = (uint8_t*) (&_dataTx);
    spiHandle->TxXferCount = 1;
    bool txAvailable = true;

    /* Check if the SPI is already enabled */
    if ((spiHandle->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
        __HAL_SPI_ENABLE(spiHandle);

    /* Transmit and Receive data in 16 Bit mode */
    spiHandle->Instance->DR = *((uint16_t*) spiHandle->pTxBuffPtr);
    spiHandle->pTxBuffPtr += sizeof(uint16_t);
    spiHandle->TxXferCount--;

    while ((spiHandle->TxXferCount > 0U) || (spiHandle->RxXferCount > 0U))
    {
        /* Check TXE flag */
        if ((__HAL_SPI_GET_FLAG(spiHandle, SPI_FLAG_TXE)) &&
            (spiHandle->TxXferCount > 0U) &&
            (txAvailable == 1U))
        {
            spiHandle->Instance->DR = *((uint16_t*) spiHandle->pTxBuffPtr);
            spiHandle->pTxBuffPtr += sizeof(uint16_t);
            spiHandle->TxXferCount--;
            /* Next Data is a reception (Rx). Tx not allowed */
            txAvailable = false;
        }
        /* Check RXNE flag */
        if ((__HAL_SPI_GET_FLAG(spiHandle, SPI_FLAG_RXNE)) && (spiHandle->RxXferCount > 0U))
        {
            *((uint16_t*) spiHandle->pRxBuffPtr) = (uint16_t) spiHandle->Instance->DR;
            spiHandle->pRxBuffPtr += sizeof(uint16_t);
            spiHandle->RxXferCount--;
            /* Next Data is a Transmission (Tx). Tx is allowed */
            txAvailable = true;
        }
    }
#endif

     M0_CS_GPIO_Port->BSRR = M0_CS_Pin;

    return dataRx;
}
