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

const char * ErrorCode[5] = {"\r\nOK\r\n", "\r\nCMDLINE_BAD_CMD\r\n", "\r\nCMDLINE_TOO_MANY_ARGS\r\n",
"\r\nCMDLINE_TOO_FEW_ARGS\r\n", "\r\nCMDLINE_INVALID_ARG\r\n" };

tCmdLineEntry g_psCmdTable[] = {{ "help", Cmd_help, " : format: help"},					
								{"set_laser", Cmd_set_laser , " : format: set_laser [int/ext] [laser_index] [dac_val]"},
								{"get_current", Cmd_get_current , " : format: get_current [int/ext]"},
								{0,0,0}
								};

volatile static	ringbuffer_t *p_CommandRingBuffer;
static	char s_commandBuffer[COMMAND_MAX_LENGTH];
static uint8_t	s_commandBufferIndex = 0;


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

void command_task_update(void*)
{
	char rxData;
	int8_t	ret_val;
	while (! rbuffer_empty(p_CommandRingBuffer))
	{
		rxData = rbuffer_remove(p_CommandRingBuffer);
		USART6_send_char(rxData);
		if ((rxData == '\r') || (rxData == '\n'))		//got a return or new line
		{
			if (s_commandBufferIndex > 0)		//if we got the CR or LF at the begining, discard	
			{
				s_commandBuffer[s_commandBufferIndex] = 0;
				s_commandBufferIndex++;
				ret_val = CmdLineProcess(s_commandBuffer);		
				s_commandBufferIndex = 0;		
				USART6_send_string(ErrorCode[ret_val]);
				USART6_send_string("> ");
			}	
			else USART6_send_string("\r\n> ");
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

	USART6_send_string("\nAvailable commands: \r\n");
	
	// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

	// Enter a loop to read each entry from the command table.  The
	// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd) {
		// Print the command name and the brief description.
		USART6_send_string(pEntry->pcCmd);
		USART6_send_string(pEntry->pcHelp);
		USART6_send_string("\r\n");

		// Advance to the next entry in the table.
		pEntry++;

	}
	// Return success.
	return (CMDLINE_OK);
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

static float cur_cal(uint16_t adc_val)
{
	float temp = (adc_val* 1000 * 3.3f)/4095.0f;	//mV
	temp /= 250;
	return temp;
}

int Cmd_get_current(int argc, char *argv[])
{
	if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
	if (argc > 2) return CMDLINE_TOO_MANY_ARGS;
	char buf[10];
	float data_temp;
	if(!strcmp(argv[1], "int"))
	{
		USART6_send_string("Current int: ");
		data_temp = cur_cal(laser_adc.ADC_Data[0]);
		UARTprintf("%s mA",ftoa((float)data_temp, buf, 2));
	}
	else if(!strcmp(argv[1], "ext"))
	{
		USART6_send_string("Current ext: ");
		data_temp = cur_cal(laser_adc.ADC_Data[1]);
		UARTprintf("%s mA",ftoa((float)data_temp, buf, 2));
	}
	else return CMDLINE_INVALID_ARG;
	return CMDLINE_OK;
}
