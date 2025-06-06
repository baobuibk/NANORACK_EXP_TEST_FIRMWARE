/*
 * ntc.c
 *
 *  Created on: Jun 6, 2025
 *      Author: DELL
 */


#include "ntc.h"

#define _NTC_R_SERIES         10000.0f
#define _NTC_R_NOMINAL        10000.0f
#define _NTC_TEMP_NOMINAL     25.0f
#define _NTC_ADC_MAX          4095 //
#define _NTC_BETA             3950


float ntc_convertToC(uint16_t adcValue)
{
	float rntc = ((double)_NTC_R_SERIES * ((double)_NTC_ADC_MAX - adcValue)) / adcValue;		//ntc pullup
	//float rntc = ((double)_NTC_R_SERIES * adcValue)/((double)_NTC_ADC_MAX - adcValue);		//ntc pulldown
	float temp;
	temp = rntc / (float)_NTC_R_NOMINAL;
	temp = logf(temp);
	temp /= (float)_NTC_BETA;
	temp += 1.0f / ((float)_NTC_TEMP_NOMINAL + 273.15f);
	temp = 1.0f / temp;
	temp -= 273.15f;
	return temp;
}

int16_t	ntc_convertToC_100times(uint16_t adcValue)
{
	return	100*ntc_convertToC(adcValue);
}
