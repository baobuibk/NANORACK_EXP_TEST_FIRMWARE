/*
 * adc_dma_device.h
 *
 *  Created on: Feb 28, 2025
 *      Author: DELL
 */

#ifndef ADC_DMA_DEVICE_H_
#define ADC_DMA_DEVICE_H_

#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_dma.h"

#define ADC_CHANNEL_MAX		16
typedef struct {
    ADC_TypeDef* ADCx;         	// ADC instance
    DMA_TypeDef* DMAx;			// DMA instance
    uint32_t stream;			// DMA_Stream (LL_DMA_STREAM_0, etc.)
    uint16_t ADC_Data[ADC_CHANNEL_MAX];
    uint32_t data_length;
} ADC_DMA_Device_t;

// Khai báo các hàm
void ADC_DMA_Init(	ADC_DMA_Device_t* ADC_DMA_Device,
					ADC_TypeDef* ADCx,
					DMA_TypeDef* DMAx,
					uint32_t stream,
					uint32_t length);

void ADC_Trigger_IRQHandler(ADC_DMA_Device_t* ADC_DMA_Device);

#endif /* ADC_DMA_DEVICE_H_ */
