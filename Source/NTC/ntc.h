/*
 * ntc.h
 *
 *  Created on: Jun 6, 2025
 *      Author: DELL
 */

#ifndef NTC_NTC_H_
#define NTC_NTC_H_

#include "main.h"
#include <math.h>
#include <stdint.h>

float ntc_convertToC(uint16_t adcValue);
int16_t ntc_convertToC_100times(uint16_t adcValue);

#endif /* NTC_NTC_H_ */
