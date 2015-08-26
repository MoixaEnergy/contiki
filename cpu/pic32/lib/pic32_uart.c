/*
 * Contiki PIC32 Port project
 *
 * Copyright (c) 2012,
 *  Scuola Superiore Sant'Anna (http://www.sssup.it) and
 *  Consorzio Nazionale Interuniversitario per le Telecomunicazioni
 *  (http://www.cnit.it).
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \addtogroup pic32 PIC32 Contiki Port
 *
 * @{
 */

/**
 * \file   pic32_uart.c
 * \brief  UART Interface for PIC32MX (pic32mx795f512l)
 * \author Giovanni Pellerano <giovanni.pellerano@evilaliv3.org>
 * \date   2012-03-21
 */

/*
 * PIC32MX795F512L - Specific Functions
 *
 * All the functions in this part of the file are specific for the
 * pic32mx795f512l that is characterized by registers' name that differ from
 * the 3xx and 4xx families of the pic32mx.
 */

#define __UART_CODE_TEST__ 0

#if __UART_CODE_TEST__
#define __USE_UART1__             1
#define __USE_UART2__             1
#define __USE_UART3__             1
#define __USE_UART4__             1
#define __USE_UART5__             1
#define __USE_UART6__             1
#endif /* __UART_CODE_TEST__ */

#include <dev/serial-line.h>

#include <pic32_uart.h>
#include <pic32_clock.h>
#include <pic32_irq.h>

#include <p32xxxx.h>

#include "contiki.h"

/*---------------------------------------------------------------------------*/
#define UART_PORT_INIT(XX, YY, ZZ)                                                                           \
  int8_t                                                                                                        \
  pic32_uart##XX##_init(uint32_t baudrate, uint16_t byte_format)                                               \
  {                                                                                                             \
    /* Disable Interrupts: RX, TX, ERR */                                                                       \
    IEC##ZZ##CLR = _IEC##ZZ##_U##XX##EIE_MASK | _IEC##ZZ##_U##XX##TXIE_MASK | _IEC##ZZ##_U##XX##RXIE_MASK;   \
    IFS##ZZ##CLR = _IFS##ZZ##_U##XX##EIF_MASK | _IFS##ZZ##_U##XX##TXIF_MASK | _IFS##ZZ##_U##XX##RXIF_MASK;   \
                                                                                                                \
    /* Clear thant Set Pri and Sub priority */                                                                  \
    IPC##YY##CLR = _IPC##YY##_U##XX##IP_MASK | _IPC##YY##_U##XX##IS_MASK;                                     \
    IPC##YY##SET = (6 << _IPC##YY##_U##XX##IP_POSITION) | (0 << _IPC##YY##_U##XX##IS_POSITION);               \
                                                                                                                \
    /* Mode Register Reset (this also stops UART) */                                                            \
    U##XX##MODE = 0;                                                                                           \
                                                                                                                \
    /* Use BRGH = 1: 4 divisor  */                                                                              \
    U##XX##MODESET = _U##XX##MODE_BRGH_MASK;                                                                  \
    U##XX##BRG  = pic32_clock_calculate_brg(4, baudrate);                                                      \
                                                                                                                \
    U##XX##MODESET = byte_format & 0x07; /* Number of bit, Parity and Stop bits */                             \
                                                                                                                \
    /* Status bits */                                                                                           \
    U##XX##STA = 0;                                                                                            \
    U##XX##STASET = _U##XX##STA_URXEN_MASK | _U##XX##STA_UTXEN_MASK; /* Enable RX and TX */                  \
                                                                                                             \
    IEC##ZZ##SET = _IEC##ZZ##_U##XX##RXIE_MASK;                                                              \
                                                                                                             \
    /* Enable UART port */                                                                                   \
    U##XX##MODESET = _U##XX##MODE_UARTEN_MASK;                                                               \
                                                                                                             \
    return UART_NO_ERROR;                                                                                    \
  }
/*---------------------------------------------------------------------------*/
#define UART_PORT(XX, YY)                        \
                                                 \
  int8_t                                         \
  pic32_uart##XX##_write(uint8_t data)           \
  {                                              \
    volatile uint8_t wait;                       \
                                                 \
    do {                                         \
      wait = U##XX##STAbits.UTXBF;               \
    } while(wait);                               \
                                                 \
    U##XX##TXREG = data;                         \
                                                 \
    return UART_NO_ERROR;                        \
  }
/*---------------------------------------------------------------------------*/

#define UART_CONSOLE_INIT(XX, YY)		      \
	void _mon_putc(char c)			      \
	{					      \
		pic32_uart##XX##_write(c);	      \
	}					      \
						      \
	void uart_console_init(uint32_t ubr)				\
	{								\
		pic32_uart##XX##_init(ubr, 0);				\
		printf("Initializing UART console @ %lubps\n", ubr);	\
	}								\
									\
	void serial_line_input_echo##XX##_byte(char c)			\
	{								\
		pic32_uart##XX##_write(c);				\
		serial_line_input_byte(c);				\
	}								\
									\
	UART_INTERRUPT(XX, YY, serial_line_input_echo##XX##_byte);

#ifdef __USE_UART1__
UART_PORT(1, 0)
UART_PORT_INIT(1, 6, 0)
#if UART_CONSOLE == 1
UART_CONSOLE_INIT(1, 0)
#endif /* UART_CONSOLE */
#endif /* __USE_UART1__ */

#ifdef __USE_UART2__
UART_PORT(2, 1)
UART_PORT_INIT(2, 8, 1)
#if UART_CONSOLE == 2
UART_CONSOLE_INIT(2, 1)
#endif /* UART_CONSOLE */
#endif /* __USE_UART2__ */

#ifdef __USE_UART3__
UART_PORT(3, 1)
UART_PORT_INIT(3, 7, 1)
#if UART_CONSOLE == 3
UART_CONSOLE_INIT(3, 1)
#endif /* UART_CONSOLE */
#endif /* __USE_UART3__ */

#ifdef __USE_UART4__
UART_PORT(4, 2)
UART_PORT_INIT(4, 12, 2)
#if UART_CONSOLE == 4
UART_CONSOLE_INIT(4, 2)
#endif /* UART_CONSOLE */
#endif /* __USE_UART4__ */

#ifdef __USE_UART5__
UART_PORT(5, 2)
UART_PORT_INIT(5, 12, 2)
#if UART_CONSOLE == 5
UART_CONSOLE_INIT(5, 2)
#endif /* UART_CONSOLE */
#endif /* __USE_UART5__ */

#ifdef __USE_UART6__
UART_PORT(6, 2)
UART_PORT_INIT(6, 12, 2)
#if UART_CONSOLE == 6
UART_CONSOLE_INIT(6, 2)
#endif /* UART_CONSOLE */
#endif /* __USE_UART6__ */

/** @} */
