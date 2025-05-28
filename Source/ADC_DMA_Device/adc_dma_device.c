/*
 * adc_dma_device.c
 *
 *  Created on: Feb 28, 2025
 *      Author: DELL
 */

#include "adc_dma_device.h"

//Giả sử ADC được cấu hình sẵn ở mode Continous, DMA request, .etc
void ADC_DMA_Init(	ADC_DMA_Device_t* ADC_DMA_Device,
					ADC_TypeDef* ADCx,
					DMA_TypeDef* DMAx,
					uint32_t stream,
					uint32_t length)
{
	ADC_DMA_Device->ADCx = ADCx;
	ADC_DMA_Device->DMAx = DMAx;
	ADC_DMA_Device->stream = stream;
	ADC_DMA_Device->data_length = length;



	LL_DMA_SetPeriphAddress(ADC_DMA_Device->DMAx, ADC_DMA_Device->stream, (uint32_t)&ADC_DMA_Device->ADCx->DR);
	LL_DMA_SetMemoryAddress(ADC_DMA_Device->DMAx, ADC_DMA_Device->stream, (uint32_t)ADC_DMA_Device->ADC_Data);
	LL_DMA_SetDataLength(ADC_DMA_Device->DMAx, ADC_DMA_Device->stream, ADC_DMA_Device->data_length);

	LL_DMA_EnableStream(ADC_DMA_Device->DMAx, ADC_DMA_Device->stream);

	while (!LL_ADC_IsEnabled(ADC_DMA_Device->ADCx))
	{
		LL_ADC_Enable(ADC_DMA_Device->ADCx);
		__NOP();
	}

    LL_ADC_REG_StartConversionSWStart(ADC_DMA_Device->ADCx);
}


void ADC_Trigger_IRQHandler(ADC_DMA_Device_t* ADC_DMA_Device)
{
	if (!LL_ADC_IsActiveFlag_EOCS(ADC_DMA_Device->ADCx))	return;

	LL_ADC_REG_StartConversionSWStart(ADC_DMA_Device->ADCx);
}


