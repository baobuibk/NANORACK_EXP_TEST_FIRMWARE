/*
 * command.c
 *
 * Created: 5/19/2024 8:37:53 PM
 *  Author: Admin
 */ 
#include "scheduler.h"
#include "command.h"
#include "UART.h"
#include <stdlib.h>
#include "cmdline.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/

const char * ErrorCode[5] = {"\r\nOK", "\r\nCMDLINE_BAD_CMD", "\r\nCMDLINE_TOO_MANY_ARGS",
"\r\nCMDLINE_TOO_FEW_ARGS", "\r\nCMDLINE_INVALID_ARG"};

tCmdLineEntry g_psCmdTable[] = {{ "help", Cmd_help, " : format: help"},					
								{"set_laser", Cmd_set_laser , " : format: set_laser [int/ext] [laser_index] [dac_val]"},
								{"get_current", Cmd_get_current , " : format: get_current [int/ext]"},
								{"pd_get", Cmd_pd_get , " : format: pd_get [pd_index]"},
								{"sp_set", Cmd_sample_set , " : format: sp_set [photo_index] [sampling_rate]"},
								{"sp_trig", Cmd_sample_trig , " : format: sp_trig [num_samples]"},
								{"sp_status", Cmd_sample_status_get , " : format: sp_status"},
								{"sp_get", Cmd_sample_get , " : format: sp_get [num_samples]"},
								{"sp_get_c", Cmd_sample_get_char , " : format: sp_get_c [num_samples]"},
								{0,0,0}
								};

volatile static	ringbuffer_t *p_CommandRingBuffer;
static	char s_commandBuffer[COMMAND_MAX_LENGTH];
static uint8_t	s_commandBufferIndex = 0;
static uint8_t photo_index = 0;
static uint32_t samp_rate = 0;

void	command_init(void)
{
	USART6_init();
	p_CommandRingBuffer = uart_get_USART6_rx_buffer_address();
	memset((void *)s_commandBuffer, 0, sizeof(s_commandBuffer));
	s_commandBufferIndex = 0;
	USART6_send_string("EXP_LASER_TEST V1.1.0 \r\n>");
}

static const double rounders[MAX_PRECISION + 1] =
{
	0.5,				// 0
	0.05,				// 1
	0.005,				// 2
};

char * ftoa(double f, char * buf, int precision)
{
	char * ptr = buf;
	char * p = ptr;
	char * p1;
	char c;
	long intPart;
	if (precision > MAX_PRECISION)
	precision = MAX_PRECISION;
	if (f < 0)
	{
		f = -f;
		*ptr++ = '-';
	}
	if (precision < 0)  
	{
		if (f < 1.0) precision = 6;
		else if (f < 10.0) precision = 5;
		else if (f < 100.0) precision = 4;
		else if (f < 1000.0) precision = 3;
		else if (f < 10000.0) precision = 2;
		else if (f < 100000.0) precision = 1;
		else precision = 0;
	}
	if (precision)
	f += rounders[precision];
	intPart = f;
	f -= intPart;

	if (!intPart)
	*ptr++ = '0';
	else
	{
		p = ptr;
		while (intPart)
		{
			*p++ = '0' + intPart % 10;
			intPart /= 10;
		}
		p1 = p;
		while (p > ptr)
		{
			c = *--p;
			*p = *ptr;
			*ptr++ = c;
		}
		ptr = p1;
	}
	if (precision)
	{
		*ptr++ = '.';
		while (precision--)
		{
			f *= 10.0;
			c = f;
			*ptr++ = '0' + c;
			f -= c;
		}
	}
	*ptr = 0;
	return buf;
}

int32_t Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void crc16_CCITT_update(uint16_t *crc, uint16_t data)
{
    uint8_t bytes[2] = {data >> 8, data & 0xFF};
    for (uint8_t i = 0; i < 2; i++)
    {
        *crc ^= bytes[i] << 8;
        for (uint8_t j = 0; j < 8; j++)
            *crc = (*crc & 0x8000) ? (*crc << 1) ^ 0x1021 : *crc << 1;
    }
}

static void htoa(uint16_t hex_value, char *output)
{
    const char hex_chars[] = "0123456789ABCDEF";
    output[0] = hex_chars[(hex_value >> 12) & 0x0F];
    output[1] = hex_chars[(hex_value >> 8) & 0x0F];
    output[2] = hex_chars[(hex_value >> 4) & 0x0F];
    output[3] = hex_chars[hex_value & 0x0F];
    output[4] = ' ';
}

static float cur_cal(uint16_t adc_val)
{
	float temp = (adc_val* 1000 * 3.3f)/4095.0f;	//mV
	temp /= 250;
	return temp;
}

void command_task_update(void*)
{
	char rxData;
	int8_t	ret_val;
	while (! rbuffer_empty(p_CommandRingBuffer))
	{
		rxData = rbuffer_remove(p_CommandRingBuffer);
		if ((rxData != '\r')&&(rxData != '\n'))	USART6_send_char(rxData);
		if (rxData == '\r')//got a return or new line
		{
			if (s_commandBufferIndex > 0)		//if we got the CR or LF at the begining, discard	
			{
				s_commandBuffer[s_commandBufferIndex] = 0;
				ret_val = CmdLineProcess(s_commandBuffer);
				s_commandBufferIndex = 0;
				if(ret_val < 5)
				{
					USART6_send_string(ErrorCode[ret_val]);
					USART6_send_string("\r\n> ");
				}
			}	
			else
			{
				if(ret_val < 5)	USART6_send_string("\r\n> ");
			}
		}
		else if ((rxData == 8) || (rxData == 127))	
		{
			if (s_commandBufferIndex > 0) s_commandBufferIndex--;
		}
		else
		{
			s_commandBuffer[s_commandBufferIndex] = rxData;
			s_commandBufferIndex ++;
			if (s_commandBufferIndex >= COMMAND_MAX_LENGTH) s_commandBufferIndex= 0;
		}
	}	
}



int Cmd_help(int argc, char *argv[]) {
	tCmdLineEntry *pEntry;

	USART6_send_string("\r\nAvailable commands:");
	pEntry = &g_psCmdTable[0];
	while (pEntry->pcCmd) {
		USART6_send_string("\r\n");
		USART6_send_string(pEntry->pcCmd);
		USART6_send_string(pEntry->pcHelp);
		pEntry++;

	}
	return CMDLINE_OK;
}

//*****************************************************************************
//
// Format: get_temp
// Get the temperature from NTC
//
//*****************************************************************************
int Cmd_set_laser(int argc, char *argv[])
{
	uint8_t laser_ind = atoi(argv[2]);
	uint8_t percent = atof(argv[3]);
	if(!strcmp(argv[1], "int"))
	{
		if(!laser_ind)
		{
			if(argc < 3)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 3)	return CMDLINE_TOO_MANY_ARGS;
			MCP4902_Shutdown(&DAC_device, MCP4902_CHA);
			ADG1414_Chain_SwitchAllOff(&laser_int);
			return CMDLINE_OK;
		}

		else if((laser_ind > 0)&&(laser_ind <= 36))
		{
			if(argc < 4)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 4)	return CMDLINE_TOO_MANY_ARGS;
			if((percent < 0)||(percent > 100))	return CMDLINE_INVALID_ARG;

			uint8_t dac_val = Map(percent, 0, 100, 0, 255);
			MCP4902_Set_DAC(&DAC_device, MCP4902_CHA, dac_val);
			ADG1414_Chain_SwitchOn(&laser_int, laser_ind);
		}
		else if(laser_ind > 36)	return CMDLINE_INVALID_ARG;

	}

	else if(!strcmp(argv[1], "ext"))
	{
		if(!laser_ind)
		{
			if(argc < 3)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 3)	return CMDLINE_TOO_MANY_ARGS;
			MCP4902_Shutdown(&DAC_device, MCP4902_CHB);
			ADG1414_Chain_SwitchAllOff(&laser_ext);
			return CMDLINE_OK;
		}

		else if((laser_ind > 0)&&(laser_ind <= 8))
		{
			if(argc < 4)		return CMDLINE_TOO_FEW_ARGS;
			else if(argc > 4)	return CMDLINE_TOO_MANY_ARGS;
			if((percent < 0)||(percent > 100))	return CMDLINE_INVALID_ARG;

			uint8_t dac_val = Map(percent, 0, 100, 0, 255);
			MCP4902_Set_DAC(&DAC_device, MCP4902_CHB, dac_val);
			ADG1414_Chain_SwitchOn(&laser_ext, laser_ind);
		}
		else if(laser_ind > 8)	return CMDLINE_INVALID_ARG;

	}

	else return CMDLINE_INVALID_ARG;
 	return CMDLINE_OK;
}

// Format: get_current [int/ext]
int Cmd_get_current(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	char buf[10];
	float data_temp;
	if(!strcmp(argv[1], "int"))
	{
		USART6_send_string("\r\nCurrent int: ");
		data_temp = cur_cal(laser_adc.ADC_Data[0]);
		UARTprintf("%s mA",ftoa((float)data_temp, buf, 2));
	}
	else if(!strcmp(argv[1], "ext"))
	{
		USART6_send_string("\r\nCurrent ext: ");
		data_temp = cur_cal(laser_adc.ADC_Data[1]);
		UARTprintf("%s mA",ftoa((float)data_temp, buf, 2));
	}
	else return CMDLINE_INVALID_ARG;
	return CMDLINE_OK;
}

//Format: pd_get [pd_index]
int Cmd_pd_get(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;

	uint8_t pd_ind = atoi(argv[1]);
	if((pd_ind < 1) || (pd_ind > 36)) return CMDLINE_INVALID_ARG;

	SPI_SetDataLength(SPI2, LL_SPI_DATAWIDTH_8BIT);
	SPI_SetPrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV16);
	ADG1414_Chain_SwitchOn(&photo_sw, pd_ind);
	LL_mDelay(10);

	ADS8327_Read_Data_Polling(&photo_adc, 1000);
	UARTprintf("\r\nPD_index[%d]: %d", pd_ind, (uint16_t)photo_adc.ADC_val);
	return CMDLINE_OK;
}


//format: sp_set [photo_index] [sampling_rate]
int Cmd_sample_set(int argc, char *argv[])
{
	if (argc < 3) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 3) return CMDLINE_TOO_MANY_ARGS;

	uint8_t pd_ind = atoi(argv[1]);
	uint32_t sp_rate = atoi(argv[2]);

	if((pd_ind < 1) || (pd_ind > 36)) return CMDLINE_INVALID_ARG;
	if((sp_rate < 1) || (sp_rate > 330000)) return CMDLINE_INVALID_ARG;

	SPI_SetDataLength(SPI2, LL_SPI_DATAWIDTH_8BIT);
	SPI_SetPrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV16);
	ADG1414_Chain_SwitchOn(&photo_sw, pd_ind);

	uint32_t AutoReload = ROUND(1000000.0f / sp_rate) - 1;
	LL_TIM_DisableIT_UPDATE(TIM1);
	LL_TIM_DisableCounter(TIM1);
	LL_TIM_SetAutoReload(TIM1, AutoReload);

	photo_index = pd_ind;
	samp_rate = sp_rate;
	return CMDLINE_OK;
}

//format: sp_trig [num_samples]
int Cmd_sample_trig(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	uint32_t num_sample = atoi(argv[1]);

	if((num_sample < 1) || (num_sample > 50000)) return CMDLINE_INVALID_ARG;

	// Prepare to collect data
	memset(adc_rec_buf, 0x00, adc_rec_total * 2);		//Clear pre buffer
	adc_ptr = adc_rec_buf;
	adc_rec_ind = 0;
	adc_rec_total = num_sample;
	SPI_SetDataLength(SPI2, LL_SPI_DATAWIDTH_16BIT);
	SPI_SetPrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV2);
	// Start collect data
	LL_TIM_SetCounter(TIM1, 0);
	LL_TIM_EnableIT_UPDATE(TIM1);  // Bật ngắt Update
	LL_TIM_EnableCounter(TIM1);    // Bật timer

	return CMDLINE_OK;
}

//format: sp_status
int Cmd_sample_status_get(int argc, char *argv[])
{
	if (argc < 1) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 1) return CMDLINE_TOO_MANY_ARGS;

	UARTprintf("\r\nPhoto: %d   Sampling_Rate: %d SPS", photo_index, samp_rate);
	if(adc_rec_ind == adc_rec_total)	USART6_send_string("\r\n-> ADC Data ready to get!");
	else USART6_send_string("\r\n-> ADC Data is not ready!");
	return CMDLINE_OK;
}

//format: sp_get [num_samples]
int Cmd_sample_get(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	if (!adc_rec_ind)
	{
		USART6_send_string("\r\nPlease send cmd 'sp_trig' first!");
		return CMDLINE_INVALID_ARG;
	}
	uint32_t num_sample = atoi(argv[1]);
	uint16_t crc_val = 0xffff;
	uint8_t bytes_temp[3];

	if((num_sample < 1) || (num_sample > 50000)) return CMDLINE_INVALID_ARG;

	uint32_t header = (0x000FFFFF & num_sample) | 0xFFF00000;
	bytes_temp[0] = (uint8_t)(header >> 16);
	bytes_temp[1] = (uint8_t)(header >> 8);
	bytes_temp[2] = (uint8_t)header;
	USART6_send_array((const char*)bytes_temp, 3);

	for(uint32_t i = 0; i < num_sample; i++)
	{
		crc16_CCITT_update(&crc_val, adc_rec_buf[i]);
		bytes_temp[0] = adc_rec_buf[i] >> 8;
		bytes_temp[1] = adc_rec_buf[i] & 0xFF;
		USART6_send_array((const char*)bytes_temp, 2);
	}
	bytes_temp[0] = crc_val >> 8;
	bytes_temp[1] = crc_val & 0xFF;
	USART6_send_array((const char*)&bytes_temp, 2);
	return CMDLINE_NOT_RETURN;
}

//format: sp_get_c [num_samples]
int Cmd_sample_get_char(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	if (!adc_rec_ind)
	{
		USART6_send_string("Please send cmd 'sp_trig' first!");
		return CMDLINE_INVALID_ARG;
	}
	uint32_t num_sample = atoi(argv[1]);
	uint16_t crc_val = 0xffff;

	if((num_sample < 1) || (num_sample > 50000)) return CMDLINE_INVALID_ARG;

	char ascii_buf[5];
	USART6_send_string("\r\n");
	for(uint32_t i = 0; i < num_sample; i++)
	{
		crc16_CCITT_update(&crc_val, adc_rec_buf[i]);
		htoa(adc_rec_buf[i], ascii_buf);
		USART6_send_array(ascii_buf, 5);
	}

	htoa(crc_val, ascii_buf);
	USART6_send_array(ascii_buf, 5);
	return CMDLINE_NOT_RETURN;
}



