/*
 * USART6.c
 *
 *  Created on: Nov 16, 2024
 *      Author: thanh
 */

#include "UART.h"

#define CONSOLE_UART_WRITE(pdata, len) USART6_send_array(pdata, len)
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

static const char * const g_pcHex = "0123456789abcdef";

usart_meta_t USART6_meta;
usart_meta_t *p_USART6_meta = &USART6_meta;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// USART FUNCTIONS

void USART6_IRQ(void) {
    uint8_t data;
    if (LL_USART_IsActiveFlag_TXE(USART6)) {
        if (!rbuffer_empty(&p_USART6_meta->rb_tx)) {
            data = rbuffer_remove(&p_USART6_meta->rb_tx);
            LL_USART_TransmitData8(USART6, (uint8_t)data);
        } else {
            LL_USART_DisableIT_TXE(USART6);
        }
    }
    if ((LL_USART_IsActiveFlag_RXNE(USART6) != RESET) && (LL_USART_IsEnabledIT_RXNE(USART6) != RESET)) {
        unsigned char data = LL_USART_ReceiveData8(USART6);

        if ((LL_USART_IsActiveFlag_ORE(USART6) != RESET) ||
            (LL_USART_IsActiveFlag_FE(USART6) != RESET) ||
            (LL_USART_IsActiveFlag_NE(USART6) != RESET)) {
            LL_USART_ClearFlag_ORE(USART6);
            LL_USART_ClearFlag_FE(USART6);
            LL_USART_ClearFlag_NE(USART6);
        } else {
            if (!rbuffer_full(&p_USART6_meta->rb_rx)) {
                rbuffer_insert(data, &p_USART6_meta->rb_rx);
            }
        }
        return;
    }
}

void USART6_init(void) {
    rbuffer_init(&p_USART6_meta->rb_tx); // Init Tx buffer
    rbuffer_init(&p_USART6_meta->rb_rx); // Init Rx buffer
    LL_USART_EnableIT_RXNE(USART6);
}

void USART6_send_char(char c) {
    while (rbuffer_full(&p_USART6_meta->rb_tx))
        ;
    rbuffer_insert(c, &p_USART6_meta->rb_tx);
    LL_USART_EnableIT_TXE(USART6);
}

void USART6_send_string(const char *str) {
    while (*str) {
        USART6_send_char(*str++);
    }
}

void USART6_send_array(const char *str, uint8_t len) {
    uint8_t udx;
    for (udx = 0; udx < len; udx++) {
        USART6_send_char(*str++);
    }
}

uint8_t USART6_rx_count(void) {
    return rbuffer_count(&p_USART6_meta->rb_rx);
}

uint16_t USART6_read_char(void) {
    if (!rbuffer_empty(&p_USART6_meta->rb_rx)) {
        return (uint16_t)rbuffer_remove(&p_USART6_meta->rb_rx);
    } else {
        return 0xFFFF; // Trả về giá trị lỗi khi buffer rỗng
    }
}

void USART6_close(void) {
    while (!rbuffer_empty(&p_USART6_meta->rb_tx))
        ; // Chờ truyền hết dữ liệu trong buffer
    LL_USART_Disable(USART6); // Tắt USART6
}

void USART6_SetBaudRate(uint32_t Baud) {
    while (!rbuffer_empty(&p_USART6_meta->rb_tx))
        ; // Chờ truyền hết dữ liệu
    LL_USART_Disable(USART6);
    LL_USART_InitTypeDef USART_InitStruct = {0};
    USART_InitStruct.BaudRate = Baud;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART6, &USART_InitStruct);
    LL_USART_ConfigAsyncMode(USART6);
    LL_USART_Enable(USART6);
}

volatile ringbuffer_t* uart_get_USART6_rx_buffer_address(void) {
    return &p_USART6_meta->rb_rx; // Trả về địa chỉ buffer Rx
}

static void UARTvprintf(const char *pcString, va_list vaArgP)
{
   volatile uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;

    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ui32Idx = 0;
            (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
            ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        CONSOLE_UART_WRITE(pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ui32Count == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ui32Count *= 10;
                    ui32Count += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Print out the character.
                    //
                    CONSOLE_UART_WRITE((char *)&ui32Value, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d and %i commands.
                //
                case 'd':
                case 'i':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((int32_t)ui32Value < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ui32Value = -(int32_t)ui32Value;

                        //
                        // Indicate that the value is negative.
                        //
                        ui32Neg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    CONSOLE_UART_WRITE(pcStr, ui32Idx);

                    //
                    // Write any required padding spaces
                    //
                    if(ui32Count > ui32Idx)
                    {
                        ui32Count -= ui32Idx;
                        while(ui32Count--)
                        {
                            CONSOLE_UART_WRITE(" ", 1);
                        }
                    }

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 10.
                    //
                    ui32Base = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ui32Value = va_arg(vaArgP, uint32_t);

                    //
                    // Reset the buffer position.
                    //
                    ui32Pos = 0;

                    //
                    // Set the base to 16.
                    //
                    ui32Base = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ui32Neg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value) &&
                         (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ui32Neg)
                    {
                        ui32Count--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ui32Neg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ui32Neg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ui32Count > 1) && (ui32Count < 16))
                    {
                        for(ui32Count--; ui32Count; ui32Count--)
                        {
                            pcBuf[ui32Pos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ui32Neg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ui32Pos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ui32Idx; ui32Idx /= ui32Base)
                    {
                        pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                    }

                    //
                    // Write the string.
                    //
                    CONSOLE_UART_WRITE(pcBuf, ui32Pos);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    CONSOLE_UART_WRITE(pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    CONSOLE_UART_WRITE("ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }
}

//*****************************************************************************

void UARTprintf(const char *pcString, ...)
{
    va_list vaArgP;
    va_start(vaArgP, pcString);
    UARTvprintf(pcString, vaArgP);
    va_end(vaArgP);
}
