/*
 * UART2.h
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

#ifndef UART_H_
#define UART_H_

#include "RingBuffer.h"
#include <stdarg.h>
#include <string.h>
#include <stm32f4xx.h>
#include <stm32f4xx_ll_usart.h>

// DEFINE RING BUFFER SIZE; MUST BE 2, 4, 8, 16, 32, 64 or 128
#define RBUFFER_SIZE 256

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART META STRUCT
typedef struct {
	volatile ringbuffer_t rb_rx;    // Rx ringbuffer
	volatile ringbuffer_t rb_tx;    // Tx ringbuffer
	volatile uint16_t usart_error;   // Holds error from RXDATAH
} usart_meta_t;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS
void USART6_init(void);
void USART6_send_char(char c);
void USART6_send_string(const char *str);
void USART6_send_array(const char *str, uint8_t len);
uint8_t USART6_rx_count(void);
uint16_t USART6_read_char(void);
void USART6_close(void);
void USART6_SetBaudRate(uint32_t Baud);
volatile ringbuffer_t* uart_get_USART6_rx_buffer_address(void);
void UARTprintf(const char *pcString, ...);
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//UART ISR
void USART6_IRQ();
void BLE_TX_Manager();

#endif /* UART_H_ */
